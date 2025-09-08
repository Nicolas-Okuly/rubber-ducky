#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
// #include <boost/algorithm/string.hpp>
#include <SFML/Graphics.hpp>

#include "./compiled_assets/single-duck.h"
#include "./compiled_assets/ducks.h"
#include "./compiled_assets/copyduck.h"



std::string randomQuote() {
    std::vector<std::string> dialogues = {
        "Quack!",
        "Got any grapes?",
        "Dumb quack",
        "Mother quacker",
        "Mmm… quack?",
        "Your code smells like toast.",
        "Debugging is hard… but I'm fluffier.",
        "Quackity quack! Just felt like saying that.",
        "I saw a fish once."
    };

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, dialogues.size() - 1);
    return dialogues[distr(gen)];
}

void drawSpeechBubble(sf::RenderWindow &window, sf::Font &font, const sf::Vector2f &position, const std::string &message) {
    const float padding = 8.f;

    // Create text
    sf::Text text(message, font, 16);
    text.setFillColor(sf::Color::Black);

    // Calculate bubble size based on text
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::RectangleShape bubble(sf::Vector2f(textBounds.width + padding * 2, textBounds.height + padding * 2));
    bubble.setFillColor(sf::Color(255, 255, 255, 230)); // Slightly transparent
    bubble.setOutlineColor(sf::Color::Black);
    bubble.setOutlineThickness(2.f);

    // Position bubble near the duck
    bubble.setPosition(position.x, position.y - bubble.getSize().y - 10);
    text.setPosition(bubble.getPosition().x + padding, bubble.getPosition().y + padding / 2);

    // Draw bubble and text
    window.draw(bubble);
    window.draw(text);
}

void handleTextInput(sf::RenderWindow & window, std::string & input) {
    // boost::algorithm::to_lower(input);
    std::cout << input << std::endl;

    if (input == "quit") window.close();
}

int main() {
    // Window setup
    const int windowWidth = 400;
    const int windowHeight = 450;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Rubber Ducky", sf::Style::None);
    sf::Color windowColor(255, 255, 255);

    // Set window icon
    sf::Image icon;
    if (!icon.loadFromMemory(single_duck_png, single_duck_png_len)) {
        std::cerr << "Failed to load icon!" << std::endl;
    } else {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    static sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    const int xPos = (desktop.width - windowWidth) - 25; 
    const int yPos = (desktop.height - windowHeight) - 50;

    window.setPosition({
        xPos, yPos
    });

    // Duck sprite setup
    int frame = 0;
    const int frameWidth = 32;
    const int frameHeight = 32;
    const int numFrames = 6;

    bool speaking = false;
    sf::Clock speechClock;
    float speechDuration = 2.5f;
    std::string quote;

    sf::Clock clock;
    sf::Texture texture;
    sf::Sprite sprite;

    if (!texture.loadFromMemory(ducks_png, ducks_png_len)) return -1;
    
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setPosition(0, 0);
    sprite.setScale(static_cast<float>(windowWidth) / frameWidth, static_cast<float>(windowHeight - 50) / frameHeight);

    // Text input setup
    sf::Font font;
    if (!font.loadFromMemory(Copyduck_ttf, Copyduck_ttf_len)) std::cerr << "Could not load font!" << std::endl;

    bool isTyping = false;
    std::string currentInput;
    const int textBoxHeight = 50;
    const float padding = 10.f;

    sf::RectangleShape textBox(sf::Vector2f(windowWidth, textBoxHeight));
    textBox.setFillColor(sf::Color::White);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);
    textBox.setPosition(0, windowHeight - textBoxHeight);

    sf::Text text("", font, 20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(textBox.getPosition().x + padding, textBox.getPosition().y + 5);

    sf::RectangleShape cursor(sf::Vector2f(2.f, 20.f));
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(text.getPosition().x, text.getPosition().y);

    sf::Clock cursorClock;
    bool showCursor = true;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Click to start typing
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (mousePos.y > window.getSize().y - textBoxHeight) {
                    isTyping = true;
                }
            }

            // Handle text input
            if (isTyping && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // Backspace
                    if (!currentInput.empty()) currentInput.pop_back();
                } else if (event.text.unicode == 13) { // Enter
                    isTyping = false;
                    handleTextInput(window, currentInput);

                    currentInput.clear();

                    quote = randomQuote();
                    speaking = true;
                    speechClock.restart();
                } else if (event.text.unicode == 27) { // Escape
                    isTyping = false;
                    currentInput.clear();
                } else if (event.text.unicode < 128) {
                    currentInput += static_cast<char>(event.text.unicode);
                }
            }
        }

        // Update visible text (auto-scroll if too long)
        std::string visibleString = currentInput;
        text.setString(visibleString);
        while (text.getLocalBounds().width > textBox.getSize().x - padding * 2) {
            visibleString.erase(0, 1);
            text.setString(visibleString);
        }

        // Update cursor
        cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 2, text.getPosition().y);

        // Blink cursor
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
        }

        // Animate duck
        if (clock.getElapsedTime().asSeconds() > 0.1f) {
            frame = (frame + 1) % numFrames;
            sprite.setTextureRect(sf::IntRect(frame * frameWidth, 0, frameWidth, frameHeight));
            clock.restart();
        }

        // Draw everything
        window.clear(windowColor);
        window.draw(sprite);
        window.draw(textBox);
        window.draw(text);
        if (isTyping && showCursor) window.draw(cursor);

        if (speaking) {
            sf::Vector2f duckPos = sprite.getPosition();
            sf::FloatRect duckBounds = sprite.getGlobalBounds();
            drawSpeechBubble(window, font, sf::Vector2f(duckPos.x, duckPos.y + duckBounds.height / 5), quote);

            // Hide bubble after duration
            if (speechClock.getElapsedTime().asSeconds() > speechDuration) {
                speaking = false;
            }
        }

        window.display();
    }

    return 0;
}
