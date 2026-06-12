#include <SFML/Graphics.hpp>

using namespace sf;

const int FILAS = 16;
const int COLUMNAS = 16;
const int TAM = 20;

int mario[FILAS][COLUMNAS] =
{
{0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
{0,0,2,2,2,3,3,2,3,0,0,0,0,0,0,0},
{0,2,3,2,3,2,3,2,3,3,3,0,0,0,0,0},
{0,2,3,2,2,3,3,3,2,3,3,3,0,0,0,0},
{0,2,2,3,3,3,3,2,2,2,2,0,0,0,0,0},
{0,0,0,3,3,3,3,3,3,3,0,0,0,0,0,0},
{0,0,1,1,4,1,1,1,0,0,0,0,0,0,0,0},
{0,1,1,1,4,1,1,4,1,1,1,0,0,0,0,0},
{1,1,1,1,4,4,4,4,1,1,1,1,0,0,0,0},
{2,2,1,4,5,4,4,5,4,1,2,2,0,0,0,0},
{2,2,2,4,4,4,4,4,4,2,2,2,0,0,0,0},
{2,2,4,4,4,4,4,4,4,4,2,2,0,0,0,0},
{0,0,4,4,4,0,0,4,4,4,0,0,0,0,0,0},
{0,5,5,5,0,0,0,0,5,5,5,0,0,0,0,0},
{5,5,5,5,0,0,0,0,5,5,5,5,0,0,0,0}
};

int main()
{
    RenderWindow window(VideoMode(800,600), "Mario Pixel Art");

    RectangleShape pixel(Vector2f(TAM, TAM));

    Color colores[6];

    colores[0] = Color::Transparent;
    colores[1] = Color::Red;
    colores[2] = Color(255,220,177);
    colores[3] = Color(139,69,19);
    colores[4] = Color::Blue;
    colores[5] = Color(139,69,19);

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        window.clear(Color(135,206,235));

        for(int i=0;i<FILAS;i++)
        {
            for(int j=0;j<COLUMNAS;j++)
            {
                if(mario[i][j] != 0)
                {
                    pixel.setFillColor(colores[mario[i][j]]);
                    pixel.setPosition(
                        250 + j*TAM,
                        120 + i*TAM
                    );
                    window.draw(pixel);
                }
            }
        }

        window.display();
    }

    return 0;
}