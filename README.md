# File Encryption Tool
This **File Encryption Tool** encrypts all files in the target folder in binary size using the **AES128** encryption standard.

## Notes
When you try to open encrypted files, some applications may corrupt these files.<br />
While the tool is running, it creates a file called **fet.fet**, which contains the addresses of the files to be encrypted and decrypted.

## Usage
### How to build the tool?
'''
g++ main.cpp "AES/aes128.cpp" "File Control/fileControl.cpp" "File Search\fileSearch.cpp" -o FileEncryptionTool.exe
'''
### How to use the tool?
**The tool performs encryption and decryption operations in the "FET" folder in the same directory.**
**The tool encrypts all data within the "FET" folder.**
**The length of the key must be 16 characters.**
**If you add -x, it will perform operations in the extensions you specify.**
*Encryption:*
'''
FileEncryptionTool.exe -e -k ENTERTHEKEY
'''
*Decryption:*
'''
FileEncryptionTool.exe -d -k ENTERTHEKEY
'''

## Technical Information
The **aes128.cpp** file encrypts and decrypts the sent **pointer data** with the help of the sent password.
'''cpp
Aes128 aes128;
aes128.Encryption(&_data, _key);
'''
'''cpp
Aes128 aes128;
aes128.Decryption(&_data, _key);
'''
Don't forget to add the extensions you want to encrypt to the **fileSearch.hpp** file.
'''cpp
std::set<std::filesystem::path> extensions = {".jpg", ".mp4", ".txt", ".par"};
'''