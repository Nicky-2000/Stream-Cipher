# Stream-Cipher

Stream-Cipher made in C++

Functions:

Encoder: Takes in a key and a pointer to a string (char array). It returns a dynamically allocated char array name "ciphertext"

Decoder: Takes in a pointer to a character array ("ciphertext") and a key. Returns the original text which was input to the Encoder ONLY if the key is the same one the text was encoded with. 
