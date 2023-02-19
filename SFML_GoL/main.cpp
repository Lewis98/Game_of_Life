#include <SFML/Graphics.hpp>
#include <vector>

#include <iostream>


struct g_state {
    bool paused;
    
    g_state() {
        paused = true;
    }
};

class point {
public:
    sf::RectangleShape disp;
    int x, y;
    bool live;
};

int main()
{
    g_state mg_state;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Conway's Game of Life");

    std::vector<point> pixels;
    const sf::Color colors[] = { sf::Color(100, 0, 0), sf::Color(0, 100, 0), sf::Color(0, 0, 100) };

    sf::Color c_live = sf::Color(250, 250, 250);
    sf::Color c_dead = sf::Color(0, 0, 0);
    
    int clock = 0;
    int inc = 1;
    int inc2 = 0;

    int pixSize = 10;
    for (int x = 0; x < floor(window.getSize().x / pixSize); x++) {
        for (int y = 0; y < floor(window.getSize().y / pixSize); y++) {
            point p;
            p.disp.setSize(sf::Vector2f(pixSize, pixSize));
            p.disp.setPosition(x * pixSize, y * pixSize);

            p.disp.setFillColor(sf::Color(0, 0, 0));

            p.x = x;
            p.y = y;

            p.live = false;

            pixels.push_back(p);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    mg_state.paused = !mg_state.paused;
                    std::cout << "Game Paused: '" << mg_state.paused << "'" << std::endl;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i m_pos = sf::Mouse::getPosition(window);
                    


                    std::cout << "CLICK AT X: " << m_pos.x << " | Y: " << m_pos.y << std::endl;

                    for (int i = 0; i < pixels.size(); i++) {
                        sf::Vector2f s_pos = pixels[i].disp.getPosition();

                        if (m_pos.x > s_pos.x && m_pos.x < s_pos.x + pixSize) {
                            if (m_pos.y > s_pos.y && m_pos.y < s_pos.y + pixSize) {
                                if (pixels[i].live) {
                                    pixels[i].disp.setFillColor(c_dead);
                                    pixels[i].live = false;
                                } else {
                                    pixels[i].disp.setFillColor(c_live);
                                    pixels[i].live = true;
                                }
                                
                            }
                        }
                    }

                }
            }
        }

        std::vector<int> toKill;
        std::vector<int> toCreate;

        if (!mg_state.paused) { // If not paused
            for (int i = 0; i < pixels.size(); i++) { // for every pixel
                
                
                int neighbours = 0;

                
                for (int y = -1; y < 2; y++) {
                    int per_col = floor(window.getSize().x / pixSize);
                    int row = floor(i / per_col) + y;

                    for (int x = -1; x < 2; x++) {

                        if (y == 0 && x == 0) continue;

                        int per_row = floor(window.getSize().y / pixSize);
                        int col = (i % per_row) + x;

                        if ((row * per_row) + col < pixels.size()) {
                            if (pixels[(row * per_row) + col].live) {
                                neighbours++;
                            };
                        }
                    }
                }


                if (pixels[i].live) {
                    if (neighbours < 2 || neighbours > 3) {
                        toKill.push_back(i);
                        std::cout << "Pixel died with '" << neighbours << "' neighbours!" << std::endl;
                    }
                }
                else {
                    if (neighbours == 3) {
                        toCreate.push_back(i);
                        std::cout << "Pixel created with '" << neighbours << "' neighbours!" << std::endl;
                    }
                }
                
            } 
        }

        for (int i = 0; i < toKill.size(); i++) {
            pixels[toKill[i]].live = false;
            pixels[toKill[i]].disp.setFillColor(c_dead);
        }

        for (int i = 0; i < toCreate.size(); i++) {
            pixels[toCreate[i]].live = true;
            pixels[toCreate[i]].disp.setFillColor(c_live);
        }

        
        if (clock == 0) {
            inc = 1;
            inc2 = (inc2 + 1) % 3;
        }
        else if (clock == 255) {
            inc = -1;
        };

        clock += inc;
        
        float currentTime = clock;
        float fadeStart = 5;
        float length = 20;

        const float ratio{ (currentTime - fadeStart) / (length - fadeStart) };

        switch (inc2) {

        case 0:
            c_live = sf::Color(static_cast<sf::Uint8>(255.f * (1.f - ratio)), 255, 255);
            break;
        case 1:
            c_live = sf::Color(255, static_cast<sf::Uint8>(255.f * (1.f - ratio)), 255);
            break;
        case 2:
            c_live = sf::Color(255, 255, static_cast<sf::Uint8>(255.f * (1.f - ratio)));
            break;
        };
        
             
            
        


        window.clear();
        
        for (int i = 0; i < pixels.size(); i++) {
            window.draw(pixels[i].disp);
        };

        window.display();

    }

    return 0;
}