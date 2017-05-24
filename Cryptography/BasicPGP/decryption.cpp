#include <string>
using std::string;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

#include <fstream>
using std::ifstream;

#include <cstdlib>
using std::exit;

#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;
using CryptoPP::DecodingResult;

#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::PK_DecryptorFilter;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::SignerFilter;
using CryptoPP::SignatureVerificationFilter;

#include <cryptopp/files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <cryptopp/hex.h>
using CryptoPP::HexDecoder;

#include <cryptopp/SecBlock.h>
using CryptoPP::SecByteBlock;

#include <cryptopp/rsa.h>
using CryptoPP::RSA;
using CryptoPP::RSASS;
using CryptoPP::InvertibleRSAFunction;
using CryptoPP::RSAES_OAEP_SHA_Decryptor;

#include <cryptopp/pssr.h>
using CryptoPP::PSS;

#include <cryptopp/sha.h>
using CryptoPP::SHA1;

#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <cryptopp/ccm.h>
using CryptoPP::CTR_Mode;

#include <assert.h>

void LoadPrivateKey( const string& filename, RSA::PrivateKey& PrivateKey )
{
    PrivateKey.Load(FileSource( filename.c_str(), true, NULL, true ).Ref());
}
void LoadPublicKey( const string& filename, RSA::PublicKey& PublicKey )
{
    PublicKey.Load(FileSource( filename.c_str(), true, NULL, true).Ref());
}

int main(int argc, char* argv[])
{
	if(argc!= 4)
	{
		cout << "Usage: ./decrypt <recipients privatekey filename> <senders publickey filename> <secure message filename>" << endl;
		return 1;
	}
	
	
	
	string message_filename, senders_publickey_filename, my_privatekey_filename;
	
	my_privatekey_filename = argv[1];
	senders_publickey_filename = argv[2];
	message_filename = argv[3];



	string key_string, iv_string, cipher_string, signature_string;
	
	ifstream file;
	file.open(message_filename);
	
	getline(file, key_string);
	getline(file, iv_string);
	getline(file, cipher_string);
	getline(file, signature_string);
	
	
	
	string cipherkey;
    {
		HexDecoder decoder1;
        decoder1.Put((byte*)key_string.data(), key_string.size());
        decoder1.MessageEnd();

        long long size1 = decoder1.MaxRetrievable();
        cipherkey.resize(size1);
        decoder1.Get((byte*)cipherkey.data(), cipherkey.size());
    }
	
	string cipheriv;
    {
		HexDecoder decoder2;
        decoder2.Put((byte*)iv_string.data(), iv_string.size());
        decoder2.MessageEnd();

        long long size2 = decoder2.MaxRetrievable();
        cipheriv.resize(size2);
        decoder2.Get((byte*)cipheriv.data(), cipheriv.size());
    }
	
	
	
	////////////////////////////////////////////////
	// RSA - Start
	////////////////////////////////////////////////
	
	////////////////////////////////////////////////
	// Load RSA Key
	RSA::PrivateKey privateKey;
	LoadPrivateKey(my_privatekey_filename, privateKey);

	////////////////////////////////////////////////
	// RSA Decryption
	RSAES_OAEP_SHA_Decryptor rsa_d( privateKey );
	AutoSeededRandomPool rng;
	
	////////////////////////////////////////////////
	// Decryption - Key
	string recoveredkey;
	StringSource ss1( cipherkey, true, new PK_DecryptorFilter( rng,
															   rsa_d,
															   new StringSink( recoveredkey )));
	
	////////////////////////////////////////////////
	// Decryption - IV
	string recoverediv;
	StringSource ss2( cipheriv, true, new PK_DecryptorFilter( rng,
															  rsa_d,
															  new StringSink( recoverediv )));
	
	////////////////////////////////////////////////
	// RSA - End
	////////////////////////////////////////////////
	
	
	
	byte key[32];
    {
        HexDecoder decoder3;
        decoder3.Put((byte*)recoveredkey.data(), recoveredkey.size());
        decoder3.MessageEnd();
        decoder3.Get(key, sizeof(key));
    }
	
	byte iv[AES::BLOCKSIZE];
	{
		HexDecoder decoder4;
		decoder4.Put((byte*)recoverediv.data(), recoverediv.size());
		decoder4.MessageEnd();
		decoder4.Get(iv, sizeof(iv));
	}
	
	string ciphertext;
    {
        HexDecoder decoder5;
        decoder5.Put((byte*)cipher_string.data(), cipher_string.size());
        decoder5.MessageEnd();

        long long size5 = decoder5.MaxRetrievable();
        ciphertext.resize(size5);
        decoder5.Get((byte*)ciphertext.data(), ciphertext.size());
    }
	
	string signature;
    {
        HexDecoder decoder6;
        decoder6.Put((byte*)signature_string.data(), signature_string.size());
        decoder6.MessageEnd();

        long long size6 = decoder6.MaxRetrievable();
        signature.resize(size6);
        decoder6.Get((byte*)signature.data(), signature.size());
    }
	
	
	
	/*********************************\
	|	   Start AES Decryption       |
	\*********************************/
	string recovered, sig;
	
	CTR_Mode< AES >::Decryption aes_d;
	aes_d.SetKeyWithIV(key, sizeof(key), iv);
	
	// Decrypt ciphertext
	StringSource s(ciphertext, true, new StreamTransformationFilter(aes_d,
																	new StringSink(recovered)));
	
	// Decrypt signature
	StringSource sjw(signature, true, new StreamTransformationFilter(aes_d,
																	 new StringSink(sig)));

	/*********************************\
	|	    End AES Decryption        |
	\*********************************/
	
	
	
	////////////////////////////////////////////////
	// Signature Verification - Start
	////////////////////////////////////////////////
	
	// Load Senders Public Key
	RSA::PublicKey publicKey;
	LoadPublicKey(senders_publickey_filename, publicKey);
	
	// Verify and Recover
	RSASS<PSS, SHA1>::Verifier verifier( publicKey );
	StringSource( recovered+signature,
				  true,
				  new SignatureVerificationFilter(verifier,
												  NULL,
												  SignatureVerificationFilter::THROW_EXCEPTION));
	
	////////////////////////////////////////////////
	// Signature Verification - End
	////////////////////////////////////////////////



	cout << "Verified signature on message" << endl;
	cout << "recovered text: " << recovered << endl;

	return 0;
}
