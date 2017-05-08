
#include <cryptopp/rsa.h>
using CryptoPP::RSA;
using CryptoPP::InvertibleRSAFunction;
using CryptoPP::RSAES_OAEP_SHA_Encryptor;
using CryptoPP::RSAES_OAEP_SHA_Decryptor;

#include <cryptopp/sha.h>
using CryptoPP::SHA1;

#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::PK_EncryptorFilter;
using CryptoPP::PK_DecryptorFilter;

#include <cryptopp/files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <cryptopp/SecBlock.h>
using CryptoPP::SecByteBlock;

#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;
using CryptoPP::DecodingResult;

#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <string>
using std::string;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <assert.h>

void LoadPublicKey( const string& filename, RSA::PublicKey& PublicKey )
{
    // DER Encode Key - X.509 key format
    PublicKey.Load(
        FileSource( filename.c_str(), true, NULL, true).Ref()
    );
}


int main(int argc, char* argv[])
{
	string plain="RSA Encryption", cipher;
	AutoSeededRandomPool rng;


	////////////////////////////////////////////////
	// Load Key
	
	RSA::PublicKey publicKey;
	LoadPublicKey("mykey.pub", publicKey);
	

	////////////////////////////////////////////////
	// RSA Encryption
	
	RSAES_OAEP_SHA_Encryptor e( publicKey );

	StringSource ss1( plain, true, new PK_EncryptorFilter( rng, e, new StringSink( cipher )));

	////////////////////////////////////////////////
	//  Print Cipher
	
	string encoded;
	HexEncoder encoder( new StringSink( encoded ), true );
	encoder.Put((byte*)cipher.data(), cipher.size());
	encoder.MessageEnd();
	
	cout << "RSA Encryption: " << encoded << endl;


	return 0;
}
