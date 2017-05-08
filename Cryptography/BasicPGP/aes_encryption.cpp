#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

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
using CryptoPP::HexDecoder;

#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <cryptopp/ccm.h>
using CryptoPP::CTR_Mode;

#include "assert.h"

int main(int argc, char* argv[])
{
	// Seed Pseudo Random Number Generator
	AutoSeededRandomPool prng;

	// Build AES Key
	byte key[32];
	prng.GenerateBlock(key, sizeof(key));

	// Build IV
	byte iv[AES::BLOCKSIZE];
	prng.GenerateBlock(iv, sizeof(iv));

	string plain;
	string cipher, encoded, recovered;
	
	// Get Message From User
	cout << "Message:" << endl;
	getline(cin, plain);

	/*********************************\
	\*********************************/

	// Pretty print key
	encoded.clear();
	StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(encoded)));
	
	cout << "key: " << encoded << endl;
	
	// Pretty print iv
	encoded.clear();
	StringSource(iv, sizeof(iv), true, new HexEncoder(new StringSink(encoded)));
	
	cout << "iv: " << encoded << endl;

	/*********************************\
	\*********************************/
	
	try
	{
		// Create the cipher text
		CTR_Mode< AES >::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);
		// The StreamTransformationFilter adds padding as required.
		StringSource(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
		
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	/*********************************\
	\*********************************/

	// Print cipher text (represenstened in its hex form)
	encoded.clear();
	StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));

	cout << "cipher text: " << encoded << endl;
	
	/*********************************\
	\*********************************/

	return 0;
}

