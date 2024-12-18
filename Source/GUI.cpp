#include "GUI.h"
#define EXIT_BUTTON "resource/Menu/ExitButton.png"

GUI::GUI()
{
	init();
}

GUI::~GUI()
{
	delete label_coins;
	delete label_lives;
	delete label_time_remaining;
	delete label_score;
	//delete exit_button;
}

Label *GUI::createLabel()
{
	return new Label();
}

void GUI::init()
{
	this->label_coins = createLabel();
	this->label_coins->setPosition(210.0f, 10.0f);

	this->label_lives = createLabel();
	this->label_lives->setPosition(50.0f, 10.0f);

	this->label_time_remaining = createLabel();
	this->label_time_remaining->setPosition(800.0f, 10.0f);

	this->label_score = createLabel();
	this->label_score->setPosition(550.0f, 10.0f);

	//exit_button = new MenuObject(EXIT_BUTTON, 850.0f, 10.0f);
	exit_button_texture.loadFromFile(EXIT_BUTTON);
	exit_button_texture.setSmooth(true);
	exit_button.setTexture(exit_button_texture);
	exit_button.setPosition(800.0f, 10.0f);
}

void GUI::setCoin(int numCoin)
{
	std::stringstream str_stream;
	str_stream << "x" << std::setw(2) << std::setfill('0') << numCoin;
	label_coins->setString(str_stream.str());
}

void GUI::setLives(int numLives)
{
	label_lives->setString("x" + std::to_string(numLives));
}

void GUI::setTimeRemaining(int time)
{
	std::stringstream str_stream;
	str_stream << "TIME\n"
			   << std::setw(3) << std::setfill('0') << time;
	label_time_remaining->setString(str_stream.str());
}

void GUI::setScore(int score)
{
	std::stringstream str_stream;
	str_stream << "SCORE\n"
			   << std::setw(6) << std::setfill('0') << score;
	label_score->setString(str_stream.str());
}

void GUI::draw(sf::RenderWindow &w)
{
	//exit_button->draw(w);
	w.draw(exit_button);
	label_coins->draw(w);
	label_lives->draw(w);
	label_time_remaining->draw(w);
	label_score->draw(w);
}

Label::Label()
{
	if (!font.loadFromFile("resource/Minecraft.ttf"))
	{
	}
	text.setFont(font);
	text.setCharacterSize(40);			 // Set character size (in pixels)
	text.setFillColor(sf::Color::White); // Set text color
	text.setStyle(sf::Text::Bold);
}

void Label::setString(const std::string &str)
{
	text.setString(str);
}

void Label::setPosition(float x, float y)
{
	text.setPosition(x, y);
}

void Label::draw(sf::RenderWindow &w)
{
	w.draw(text);
}