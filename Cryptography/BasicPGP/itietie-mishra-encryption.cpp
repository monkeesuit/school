#include <fstream>
using std::ofstream;
using std::ifstream;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;

#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;

#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::PK_EncryptorFilter;
using CryptoPP::SignerFilter;
using CryptoPP::SignatureVerificationFilter;

#include <cryptopp/files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <cryptopp/ccm.h>
using CryptoPP::CTR_Mode;

#include <cryptopp/rsa.h>
using CryptoPP::RSA;
using CryptoPP::RSAES_OAEP_SHA_Encryptor;
using CryptoPP::RSASS;
using CryptoPP::InvertibleRSAFunction;

#include <cryptopp/sha.h>
using CryptoPP::SHA1;

#include <cryptopp/SecBlock.h>
using CryptoPP::SecByteBlock;

#include <cryptopp/pssr.h>
using CryptoPP::PSS;

#include "assert.h"

void LoadPublicKey( const string& filename, RSA::PublicKey& PublicKey )
{
    PublicKey.Load(FileSource( filename.c_str(), true, NULL, true).Ref());
}
void LoadPrivateKey( const string& filename, RSA::PrivateKey& PrivateKey )
{
    PrivateKey.Load(FileSource( filename.c_str(), true, NULL, true).Ref());
}


int main(int argc, char* argv[])
{
	if(argc!= 4)
	{
		cout << "Usage: ./encrypt <recipients publickey filename> <senders privatekey filename> <message filename>" << endl;
		return 1;
	}
	
	
	
	// Get Key and Message From User
	string recipients_publickey_filename, message_filename, my_privatekey_filename;
	
	recipients_publickey_filename	= argv[1];
	my_privatekey_filename = argv[2];
	message_filename = argv[3];
	
	
	
	// Read in message.
	string message;
	string line;
	
	ifstream readfile;
	readfile.open(message_filename);
	
	if (readfile.is_open())
	{
		while ( getline (readfile,line) )
		{
			message = message + line;
		}
		readfile.close();
	}
	
	
	
	////////////////////////////////////////////////
	// START DIGITAL SIGNATURE
	////////////////////////////////////////////////
	
	// Load My Private Key
	RSA::PrivateKey privateKey;
	LoadPublicKey(my_privatekey_filename, privateKey);
	
	// Pseudo Random Number Generator
	AutoSeededRandomPool prng_sig;
	
	// Sign and Encode
	RSASS<PSS, SHA1>::Signer signer( privateKey );

	string signature;
	StringSource( message,
				  true,
				  new SignerFilter( prng_sig,
									signer,
									new StringSink( signature )));
	
	////////////////////////////////////////////////
	// END DIGITAL SIGNATURE
	////////////////////////////////////////////////
	
	
	
	/*********************************\
	|			Start AES		      |
	\*********************************/
	
	// Pseudo Random Number Generator
	AutoSeededRandomPool prng;

	// Build AES Key
	byte key[32];
	prng.GenerateBlock(key, sizeof(key));

	// Build IV
	byte iv[AES::BLOCKSIZE];
	prng.GenerateBlock(iv, sizeof(iv));

	/*********************************\
	\*********************************/

	// print key
	string key_string;
	StringSource(key,
				sizeof(key),
				true,
				new HexEncoder(new StringSink(key_string)));
	
	// print iv
	string iv_string;
	StringSource(iv,
				 sizeof(iv),
				 true,
				 new HexEncoder(new StringSink(iv_string)));
	
	/*********************************\
	\*********************************/

	CTR_Mode< AES >::Encryption aes_e;
	aes_e.SetKeyWithIV(key, sizeof(key), iv);
	
	// Encrypt the Message
	string aes_cipher;
	StringSource(message,
				 true,
				 new StreamTransformationFilter(aes_e,
												new StringSink(aes_cipher)));
	
	// Encrypt the Signature
	string aes_sig;
	StringSource(signature,
				 true,
				 new StreamTransformationFilter(aes_e,
												new StringSink(aes_sig)));
	
	/*********************************\
	|			END AES				  |
	\*********************************/



	////////////////////////////////////////////////
	// START RSA
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	// Load RSA Key
	
	RSA::PublicKey publicKey;
	LoadPublicKey(recipients_publickey_filename, publicKey);
	
	////////////////////////////////////////////////
	// RSA Encryption
	
	RSAES_OAEP_SHA_Encryptor rsa_e( publicKey );
	
	////////////////////////////////////////////////
	// RSA Encryption - Key String
	string rsa_key_cipher;

	AutoSeededRandomPool rng1;
	
	StringSource ss1( key_string,
					  true,
					  new PK_EncryptorFilter( rng1,
											  rsa_e,
											  new StringSink( rsa_key_cipher )));
	

	////////////////////////////////////////////////
	// RSA Encryption - IV String
	string rsa_iv_cipher;

	AutoSeededRandomPool rng2;
	
	StringSource ss2( iv_string,
					  true,
					  new PK_EncryptorFilter( rng2,
											  rsa_e,
											  new StringSink( rsa_iv_cipher )));

	////////////////////////////////////////////////
	// END RSA
	////////////////////////////////////////////////
	
	
	
	// Make Results
	string encoded;
	ofstream file;
	file.open("secure_message.txt");
	
	HexEncoder encoder( new StringSink( encoded ), true );
	
	// Put AES Key Encrypted With RSA in File
	encoded.clear();
	encoder.Put((byte*)rsa_key_cipher.data(), rsa_key_cipher.size());
	encoder.MessageEnd();
	
	file << encoded << endl;
	
	// Put AES IV Encrypted With RSA in File
	encoded.clear();
	encoder.Put((byte*)rsa_iv_cipher.data(), rsa_iv_cipher.size());
	encoder.MessageEnd();
	
	file << encoded << endl;

	// Put Ciphertext Encrypted With AES in File
	encoded.clear();
	StringSource(aes_cipher, true, new HexEncoder(new StringSink(encoded)));

	file << encoded << endl;
	
	// Put Signature Encrypted With AES in File
	encoded.clear();
	StringSource(signature, true, new HexEncoder(new StringSink(encoded)));

	file << encoded << endl;
	
	file.close();

	return 0;
}

