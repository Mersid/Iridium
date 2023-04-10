#include <fstream>
#include <iostream>
#include <chrono>
#include <random>
#include <filesystem>
#include "ChunkedPpmTextureSerializer.h"
#include "BS_thread_pool.hpp"

void ChunkedPpmTextureSerializer::serialize(Texture &texture, const std::string &file) {

    // Seed the random number generator with the current time
    std::mt19937 mt = std::mt19937(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	std::uniform_int_distribution<unsigned long long> dist = std::uniform_int_distribution<unsigned long long>();
    unsigned long long number = dist(mt);

    // Create a temporary directory to store the chunks, with a random number appended to allow multiple instances to run at once
    std::filesystem::path tempDir = std::filesystem::temp_directory_path() / ("Iridium_" + std::to_string(number));
    std::filesystem::create_directories(tempDir);

    std::cout << "PPM serializing chunks to " << tempDir.string() << std::endl;

    // To lock the timer and print
    std::mutex timerMutex;
    // Number of lines completed
    std::atomic_uint progress = 0;

    BS::thread_pool pool;

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

    std::vector<std::future<void>> futures;
    futures.reserve(texture.getHeight());
    for (int y = 0; y < texture.getHeight(); y++) {
        std::future f = pool.submit([&, y]() {
            std::ofstream fileStream;
            fileStream.open(tempDir / ("chunk_" + std::to_string(y) + ".ppm"));

            for (int x = 0; x < texture.getWidth(); x++) {
                if (std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::high_resolution_clock::now() - t0).count() >= 250)
                {
                    if (timerMutex.try_lock())
                    {
                        std::cout << "\r" + std::to_string(progress) + "/" + std::to_string(texture.getHeight()) +
                                     " (" + std::to_string((double) progress / texture.getHeight() * 100) +
                                     "%)" << std::flush;

                        t0 = std::chrono::high_resolution_clock::now();
                        timerMutex.unlock();
                    }
                }

                Eigen::Vector4d pixel = texture.getPixelVectorAt(x, y);
                fileStream << (int) (pixel.x() * 255) << " " << (int) (pixel.y() * 255) << " " << (int) (pixel.z() * 255) << "\n";
            }
            fileStream.close();
            progress++;
        });

        futures.push_back(std::move(f));
    }

    for (auto& f : futures)
    {
        f.wait();
    }

    std::cout << "\n" << std::endl;

    // Write the chunks to the main file
    std::cout << "PPM consolidating chunks to " << file << std::endl;

    std::ofstream fileStream;
    fileStream.open(file);
    fileStream << "P3\n";
    fileStream << texture.getWidth() << " " << texture.getHeight() << "\n";
    fileStream << "255\n"; // Max color value

    for(int y = 0; y < texture.getHeight(); y++) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::high_resolution_clock::now() - t0).count() >= 250)
            {
                std::cout << "\r" + std::to_string(y) + "/" + std::to_string(texture.getHeight()) +
                             " (" + std::to_string((double) y / texture.getHeight() * 100) +
                             "%)" << std::flush;

                t0 = std::chrono::high_resolution_clock::now();
            }

        std::ifstream chunkStream;
        chunkStream.open(tempDir / ("chunk_" + std::to_string(y) + ".ppm"));
        std::string line;
        while (std::getline(chunkStream, line))
        {
            fileStream << line;
        }
        chunkStream.close();

        std::filesystem::remove(tempDir / ("chunk_" + std::to_string(y) + ".ppm"));
    }

    fileStream.close();

    std::filesystem::remove(tempDir);

    std::cout << "\n" << std::endl;
}

