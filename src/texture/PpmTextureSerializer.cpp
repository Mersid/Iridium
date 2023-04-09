#include <fstream>
#include <iostream>
#include "PpmTextureSerializer.h"
#include <chrono>

void PpmTextureSerializer::serialize(Texture &texture, const std::string &file) {
    std::cout << "PPM serializing to " << file << std::endl;

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();


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
        for (int x = 0; x < texture.getWidth(); x++) {

            Eigen::Vector4d pixel = texture.getPixelVectorAt(x, y);
            fileStream << (int) (pixel.x() * 255) << " " << (int) (pixel.y() * 255) << " " << (int) (pixel.z() * 255)
                       << "\n";
        }
    }

    fileStream.close();
    std::cout << "\n" << std::endl;
}

