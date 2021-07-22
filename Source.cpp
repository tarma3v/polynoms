#include "interface.h"
#include "calculator.h"


int with = 1200, height = 620;

int main()
{
	//setlocale(LC_ALL, "Russian");
	sf::RenderWindow window(sf::VideoMode(with, height), "CalculatorPolinom");
	sans.loadFromFile("sans.ttf");
	design cal;

	inputstring enter(50, "Enter Polynom");
	enter.move(25, 25);
	cal.add(&enter);

	buttontextbox operUm(50, " * ");
	buttontextbox operDerev(50, "f'(x)");
	buttontextbox operSum(50, " + ");
	buttontextbox operRaz(50, " -- ");
	operSum.move(50, 250);
	operRaz.move(50, 330);
	operUm.move(150, 250);
	operDerev.move(150, 330);

	operSum.move(0, height - 410);
	operRaz.move(0, height - 410);
	operUm.move(0, height - 410);
	operDerev.move(0, height - 410);

	cal.add(&operSum);
	cal.add(&operRaz);
	cal.add(&operDerev);
	cal.add(&operUm);




	vectortextbox input(50);
	input.move(25,height / 2 - 12);
	input.add("HELLO");
	cal.add(&input);

	listbox listpolinom(200, height - 110, 30);
	listpolinom.move(with - 220, 55);
	cal.add(&listpolinom);

	Clock clock;

	


	Polynom_database* base_head = nullptr;
	vector<int> command;


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		{
			//фон
			RectangleShape fon(Vector2f(with,height));
			fon.setFillColor({ 200,200,200 });
			window.draw(fon);
			//Мышь
			Vector2i pixelPos = Mouse::getPosition(window);
			mousepos = window.mapPixelToCoords(pixelPos);
			isClick = 0;
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					isClick = 1;
			//текст с клавиатуры
			pressButton = 0;
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128)
				{
					pressButton = static_cast<char>(event.text.unicode);
				}
			}
			timefps = clock.getElapsedTime().asMicroseconds();
			clock.restart();
			timefps /= 800;
			moveCol = 0;
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				moveCol = event.mouseWheelScroll.delta;
			}



		}


		if (enter.isEnter())
		{
			if (is_correct(enter.text))
			{
				Polynom* p = make_polynom(enter.text);
				listpolinom.add(show_polynom(p, 'X'));
				push_back_in_base(p, base_head);
			}
		}
		int h = listpolinom.isClick();
		if (h != -1 && command.size() < 2)
		{
			command.push_back(h);
			string s = listpolinom.texts[h].text;
			if (s.size() > 20)
			{
				while (s.size() > 17)
					s.pop_back();
				for (int i = 0; i < 3; i++)
					s.push_back('.');
			}
			input.add(s);
		}

		if (operSum.click() && command.size() == 2)
		{
			listpolinom.add(show_polynom(polynom_sum(command[0], command[1], base_head), 'X'));

			command.clear();
			input.texts.clear();
		}
		if (operUm.click() && command.size() == 2)
		{
			listpolinom.add(show_polynom(polynom_mul(command[0], command[1], base_head), 'X'));
			command.clear();
			input.texts.clear();
		}
		if (operRaz.click() && command.size() == 2)
		{
			listpolinom.add(show_polynom(polynom_minus(command[0], command[1], base_head), 'X'));
			command.clear();
			input.texts.clear();
		}
		if (operDerev.click() && command.size() == 1)
		{
			Polynom* p = derivative(command[0], base_head);
			if (p != nullptr)
				listpolinom.add(show_polynom(p, 'X'));
			command.clear();
			input.texts.clear();
		}
		cal.draw(window);
		window.display();
		sleep(milliseconds(10));
	}

	return 0;
}

