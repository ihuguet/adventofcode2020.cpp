#include <iostream>
#include <string>

uint_fast32_t crack_loop_size (uint_fast32_t subject, uint_fast32_t result);
uint_fast32_t calc_encrypt_key (uint_fast32_t subject, uint_fast32_t loop_size);

int main () {
    uint_fast32_t card_pubkey = 8335663;
    uint_fast32_t door_pubkey = 8614349;

    uint_fast32_t card_loop_size = crack_loop_size(7, card_pubkey);
    std::cout << "Card loop size: " << card_loop_size << std::endl;

    uint_fast32_t encryption_key = calc_encrypt_key(door_pubkey, card_loop_size);
    std::cout << "Encryption key: " << encryption_key << std::endl;
    return 0;
}

uint_fast32_t crack_loop_size (uint_fast32_t subject, uint_fast32_t result) {
    uint_fast32_t val = 1, loop = 0;
    while (val != result) {
        val *= subject;
        val %= static_cast<uint_fast32_t>(20201227);
        loop++;
    }
    return loop;
}

uint_fast32_t calc_encrypt_key (uint_fast32_t subject, uint_fast32_t loop_size) {
    uint_fast32_t val = 1;
    for (uint_fast32_t i = 0; i < loop_size; i++) {
        val *= subject;
        val %= static_cast<uint_fast32_t>(20201227);
    }
    return val;
}
