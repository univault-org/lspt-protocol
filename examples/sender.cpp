// #include <chrono>
// #include <openssl/md5.h>

// // ... (existing code)

// auto start = std::chrono::high_resolution_clock::now();
// session.sendPackage(buffer.data(), buffer.size());
// auto end = std::chrono::high_resolution_clock::now();

// std::chrono::duration<double> diff = end - start;
// std::cout << "Time to send: " << diff.count() << " s" << std::endl;

// // Calculate MD5 hash of sent file
// unsigned char result[MD5_DIGEST_LENGTH];
// MD5(reinterpret_cast<unsigned char*>(buffer.data()), buffer.size(), result);
// std::cout << "MD5 hash of sent file: ";
// for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
//     printf("%02x", result[i]);
// std::cout << std::endl;

// // Similar changes in receiver.cpp