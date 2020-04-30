from base64 import b64decode
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from Crypto.Hash import SHA256


key_digest = 'ea7587a91870f0797095aa7af784fee4f670cdfc95244ef3388530600108bf31'
iv = b64decode('oL+nS27XVkbfEgzSGKf6nw==')
flag = b64decode('ZWlGlSglZvxAsfgeRPCiqT+WDchfuuYI2cCl3ItB+qZhq2VNP6vsrA985v/VRrm0')

pswd = input().encode()
pswd_digest = SHA256.new(pswd).hexdigest()


if(key_digest == pswd_digest):
    cipher = AES.new(pswd, AES.MODE_CBC, iv)
    pt = unpad(cipher.decrypt(flag), AES.block_size)
    print('You did it: ', pt.decode('utf-8'))
else:
    print('lol nope')
