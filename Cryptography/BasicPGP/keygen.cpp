
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

#include <string>
using std::string;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <assert.h>

void SavePublicKey( const RSA::PublicKey& PublicKey, const string& filename )
{
    PublicKey.Save(FileSink( filename.c_str(), true).Ref());
}

void SavePrivateKey( const RSA::PrivateKey& PrivateKey, const string& filename )
{
    PrivateKey.Save(FileSink( filename.c_str(), true).Ref());
}

int main(int argc, char* argv[])
{
	////////////////////////////////////////////////
	// Generate keys
	AutoSeededRandomPool rng;

	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize( rng, 4096 );

	RSA::PrivateKey privateKey( params );
	RSA::PublicKey publicKey( params );
	
	SavePublicKey(publicKey, "mykey.pub");
	SavePrivateKey(privateKey, "mykey.pri");
	
	return 0;
}
