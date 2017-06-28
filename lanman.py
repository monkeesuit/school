import pyDes
import codecs

STRING = 'KGS!@#$%'



def lanman(password):

    password = password.upper()
    password += '\0' * 14
    password = password[0:14]

    key1 = password[:7]
    key2 = password[7:]

    return lan_hash(key1) + lan_hash(key2)



def lan_hash(key):

    key = key_expansion(key)

    encryptor = pyDes.des(key)
    cipher = encryptor.encrypt(STRING)

    return codecs.encode(cipher, 'hex')



def key_expansion(key):

    bitstream = ''.join(["{:08b}".format(ord(ch)) for ch in key])

    sliced_bits_array = []
    for i in range(0,len(bitstream),7):
        sliced_bits = int(bitstream[i:i+7] + '0', 2).to_bytes(1, byteorder='big')
        sliced_bits_array.append(sliced_bits)

    return b''.join(sliced_bits_array)



def run():
    print(lanman('HelloWo'))

if __name__ == "__main__":
    run()
