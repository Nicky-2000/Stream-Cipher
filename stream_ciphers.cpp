//
//  main.cpp
//  Project2
//
//  Created by Nicholas Khorasni on 2019-11-07.
//  Copyright © 2019 Nicholas Khorasni. All rights reserved.
//

#include <iostream>
#include <cmath>

char *encode(char *plaintext, unsigned long key);

char *decode (char *ciphertext, unsigned long key);


/////////////////////////
//MAIN USED FOR TESTING//
/////////////////////////

int main(int argc, const char * argv[])
{
    char str0[]{"Hello world!"};
    
    char* ciphertext = encode(str0, 51323);
    
    std::cout << ciphertext << std::endl;
    
    char* plaintext = decode(ciphertext, 51323);
    
    std::cout << plaintext << std::endl;
    
    delete [] ciphertext;
    delete [] plaintext;
    
    ciphertext = nullptr;
    plaintext = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////
// ENCODER FUNCTION -- Takes in a key and a dynamically allocated character array // 
////////////////////////////////////////////////////////////////////////////////////

char *encode(char *plaintext, unsigned long key)
{   
    int y {0};
    int size_of{0};
    
    while(plaintext[y] != '\0')
    {
        y++;
    }
    
    size_of = y;
    int number_of_printable = y;
    y %= 4;
    int number_of_nulls;
    if (y == 0)
        number_of_nulls = 0;
    else 
        number_of_nulls = 4-y;
    
    size_of += number_of_nulls;
    
    // TEST LINE: std::cout << "number of printable: " << number_of_printable <<" size of: " << size_of << " y: " <<y <<std::endl;
    
    unsigned int array_mul_of_four[size_of];
    
    for (int x {0}; x < size_of; x++)
    {
        if (number_of_nulls == 0)
        {
            array_mul_of_four[x] = plaintext[x];
        }
        else
        {
            if (x >= number_of_printable)
            {
                array_mul_of_four[x] = '\0';
            }
            else
            {
                array_mul_of_four[x] = plaintext[x];
            }
        }
    }
    unsigned char state[256];
    for (short i {0}; i < 256; i++)
    {
        state[i] = i;
    }

    unsigned char i {0};
    unsigned char j {0};
    unsigned char k {0};
    unsigned char temp_v{0};
    
    unsigned long mask{0};
    int kth_bit;

    for (int x {0}; x < 256; x++)
    {
        k = (i % 64);
        //find kth bit
        mask = 1;
        if (((mask << k)&key) > 0)
        {
            kth_bit = 1;
        }
        else
        {
            kth_bit = 0;
        }

        j = (j + state[i] + kth_bit) % 256;

        //swap state[i] and state[j]
        temp_v = state[i];
        state[i] = state[j];
        state[j] = temp_v;
        i = (i + 1) % 256;
    }
    
    for (int i {0}; i < 256; i++)
          {
              std::cout << static_cast<unsigned int> (state[i]) << " ";
          }
       std::cout << std::endl;
    
    unsigned int R{0};
    int r {0};
    unsigned char bytes_to_xor[size_of];
    
    for (int u {0}; u < size_of; u++)
    {
        i = (i + 1) % 256;
        j = (j + state[i]) % 256;
        //Swap
        temp_v = state[i];
        state[i] = state[j];
        state[j] = temp_v;
        
        r = (state[i] + state[j]) % 256;
        R = state[r];
        
        bytes_to_xor[u] = (array_mul_of_four[u]^R);
       
    }
    
    for (int i {0}; i < size_of; i++)
          {
              std::cout << static_cast<unsigned int> (bytes_to_xor[i]) << " ";
          }
       std::cout << std::endl;
    
    
    // find a number for bytes 0-3 then bytes 4-7... --> convert this number to base 85... rearrange so that least significant base 85 representation (for first 4 bytes) is at ciphertext[4]... then ciphertext[9]...
    // add null character to the end of this array
    
    int blocks = size_of/4;
    std::cout <<"blocks: " << blocks<< std::endl;
    
    int size_of_base_eighty_five = (5 * blocks) + 1;
    std::cout <<"size of base 85: " << size_of_base_eighty_five<< std::endl;
    
    char *ciphertext = new char [size_of_base_eighty_five];
    unsigned char array_eight_five[size_of_base_eighty_five];
    //ASCii armour
    
    unsigned int thirty_two_bits{0};
    int mul_of_five{0};
    for (int k {0}; k < blocks; k++)
    {
        thirty_two_bits = 0;
        thirty_two_bits = bytes_to_xor[4*k]^thirty_two_bits;
        thirty_two_bits = (thirty_two_bits << 8);
        thirty_two_bits = bytes_to_xor[4*k + 1 ]^thirty_two_bits;
        thirty_two_bits = (thirty_two_bits << 8);
        thirty_two_bits = bytes_to_xor[4*k + 2]^thirty_two_bits;
        thirty_two_bits = (thirty_two_bits << 8);
        thirty_two_bits = bytes_to_xor[4*k + 3]^thirty_two_bits;
    
        for (int t {0}; t < 5; t++)
        {
            if (thirty_two_bits == 0)
            {
                array_eight_five[t] = 0;
            }
            else
            {
                array_eight_five[t] = (thirty_two_bits % 85);
                thirty_two_bits /= 85;
            }
        }
        
        for (int h{0}; h < 5; h++)
        {
            if (mul_of_five < size_of_base_eighty_five)
            {
                ciphertext[mul_of_five] = array_eight_five[4 - h] + '!';
                mul_of_five++;
            }
        }
    }
    ciphertext[size_of_base_eighty_five - 1] = '\0';
    
    return ciphertext;
}




////////////////////////////////////////////////////////////////////////////////////
// DECODER FUNCTION -- Takes in a key and a dynamically allocated character array // 
////////////////////////////////////////////////////////////////////////////////////

char *decode (char *ciphertext, unsigned long key)
{
    
    // find lenght of printable characters in cipher text
    unsigned int cipher_length {0};
    while (ciphertext[cipher_length] != '\0')
    {
        cipher_length++;
    }

   #Test Line: std::cout << "Cipher Length: " << cipher_length << std::endl;
    
    unsigned long num_of_thirty_two_nums = (cipher_length / 5);
    unsigned int array_of_thirty_two [num_of_thirty_two_nums];
    unsigned int starting_value {0};
    for (int x {0}; x < num_of_thirty_two_nums; x++)
    {
        array_of_thirty_two[x] = pow(85, 4)*( ciphertext[starting_value] - '!')
                               + pow(85, 3)*( ciphertext[starting_value + 1] - '!')
                               + pow(85, 2)*( ciphertext[ starting_value + 2] - '!')
                               + pow(85, 1)*( ciphertext[ starting_value + 3] - '!')
                               + pow(85, 0)*( ciphertext[ starting_value + 4] - '!');
        starting_value += 5;
    }
    
    for (int x {0}; x < num_of_thirty_two_nums; x++)
    {
        std::cout << array_of_thirty_two[x] << std::endl;
    }
    
    unsigned char array_to_xor [4*num_of_thirty_two_nums];
    unsigned char value = {0};
    for (int i {0}; i < num_of_thirty_two_nums; i++)
    {
        array_to_xor[i*4 + 0] = (array_of_thirty_two[i] >> 24)^value;
        array_to_xor[i*4 + 1] = (array_of_thirty_two[i] >> 16)^value;
        array_to_xor[i*4 + 2] = (array_of_thirty_two[i] >> 8 )^value;
        array_to_xor[i*4 + 3] = (array_of_thirty_two[i] >> 0 )^value;
    }
    
    // initialize state array again
    
    unsigned char state[256];
    for (short i {0}; i < 256; i++)
    {
        state[i] = i;
    }
    // randomize state array
    unsigned char i {0};
    unsigned char j {0};
    unsigned char k {0};
    unsigned char temp_v{0};
    unsigned long mask{0};
    int kth_bit;

    for (int x {0}; x < 256; x++)
    {
        k = (i % 64);
        //find kth bit
        mask = 1;
        if (((mask << k)&key) > 0)
        {
            kth_bit = 1;
        }
        else
        {
            kth_bit = 0;
        }
        j = (j + state[i] + kth_bit) % 256;
        //swap state[i] and state[j]
        temp_v = state[i];
        state[i] = state[j];
        state[j] = temp_v;
        i = (i + 1) % 256;
    }
    
    unsigned int R{0};
    int r {0};
    char* decoded = new char [4*num_of_thirty_two_nums + 1];
    for (int u {0}; u < 4*num_of_thirty_two_nums; u++)
    {
        i = (i + 1) % 256;
        j = (j + state[i]) % 256;
        //Swap
        temp_v = state[i];
        state[i] = state[j];
        state[j] = temp_v;
        
        r = (state[i] + state[j]) % 256;
        R = state[r];
        
        decoded[u] = (array_to_xor[u]^R);
    }
    
    decoded[4*num_of_thirty_two_nums + 1] = '\0';
    
    return decoded;
}
