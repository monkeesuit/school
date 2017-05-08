
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

#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

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

void LoadPrivateKey( const string& filename, RSA::PrivateKey& PrivateKey )
{
    // DER Encode Key - PKCS #8 key format
    PrivateKey.Load(
        FileSource( filename.c_str(), true, NULL, true /*binary*/ ).Ref()
    );
}

int main(int argc, char* argv[])
{
	AutoSeededRandomPool rng;
	string recovered;

	RSA::PrivateKey privateKey;
	LoadPrivateKey("mykey.pri", privateKey);
	
	string cipher_string = "9850D27B4D4FC37E73D500B642A29D9ABBB85601D5E3664A60FB55F9145E90C80CAE697329BFC4890B93862A79374A4ADA46FBA304CC21B36AAE1F68BA02E90D0DF8AA38F7ADD42184F7F91F1F62C6B4F00B97D86581E8C2572A3267EE92229287E35B473BAB98C04B3836639CEF1FF59DAEFCE30EDEBC57CD71834C00E8A963227B9E00984758125528BA953FA648A1F0B10E79D3D6DC05C87E15727018599A9FA9F8A6F615908FBA8417B27CDC83F8E79AB9B1D11BD8E6A431B91A56777E2398F7F58103D1D44FDB17FEF68149A2C7705C7708C52D194B3489CE80A90E767C22AD69061BFAC38BCA6CB30D75323A74002A435DFC69759F8ACA2975523A4DD37F4E2063C92F902461565E91EB8F36FEE72D8AADD69B9161B38DED69E2A32E248B867B187D09321E85F842D0462E33750C31E412ED5787026CCEBF234C79BF1100E225E9636E448F13BB7F00CF48B3C7E9778E61EDE70A718F968D5C74B4D895C677A20EF0D8E8E6B8D780EC346314BFD69D09B8F1BCAA7987B50796E6DD15C5203888AA5777F6069F17ABC309A5D0FF3693BA5C5EFD5640A432720195B86258EBAAD5E7C0AD5EB6BD693190BFB78401846A80E17E40873DE8C1A1BC811B82A44DE31AA1BDE459B00FC0882B5D41F51FBFF4FB5EA3270D9A63B18BBEDBC9F575406E61415E5CCD8F4A6CEFA9F6C55BAB75FAAEC306D4FBDDE3BA8CA685487668";
	
	string cipher;
    {
        HexDecoder decoder;
        decoder.Put((byte*)cipher_string.data(), cipher_string.size());
        decoder.MessageEnd();

        long long size = decoder.MaxRetrievable();
        cipher.resize(size);
        decoder.Get((byte*)cipher.data(), cipher.size());
    }
	

	////////////////////////////////////////////////
	// Decryption
	RSAES_OAEP_SHA_Decryptor d( privateKey );

	StringSource ss2( cipher, true, new PK_DecryptorFilter( rng, d, new StringSink( recovered )));

	 cout << "Recovered: " << recovered	<< endl;

	return 0;
}
