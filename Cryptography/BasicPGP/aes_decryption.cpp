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
    string key_string, iv_string, cipher_string;
	string recovered;
	
	cout << "Key: ";
	getline(cin, key_string);
	cout << "IV: ";
	getline(cin, iv_string);
	cout << "Cipher Text: ";
	getline(cin, cipher_string);

	byte iv[AES::BLOCKSIZE];
	{
		HexDecoder decoder;
		decoder.Put((byte*)iv_string.data(), iv_string.size());
		decoder.MessageEnd();
		decoder.Get(iv, sizeof(iv));
	}
	
	string cipher;
    {
        HexDecoder decoder;
        decoder.Put((byte*)cipher_string.data(), cipher_string.size());
        decoder.MessageEnd();

        long long size = decoder.MaxRetrievable();
        cipher.resize(size);
        decoder.Get((byte*)cipher.data(), cipher.size());
    }
	
	byte key[32];
    {
        HexDecoder decoder;
        decoder.Put((byte*)key_string.data(), key_string.size());
        decoder.MessageEnd();
        decoder.Get(key, sizeof(key));
    }

	/*********************************\
	\*********************************/
	
	try
	{
		// Decrypt cipher text
		CTR_Mode< AES >::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);
		// The StreamTransformationFilter removes padding as required.
		StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(recovered)));

		cout << "recovered text: " << recovered << endl;
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	/*********************************\
	\*********************************/

	return 0;
}

