#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

bool rectBrush = true;
bool circleBrush = false;

double percentageToDraw = (rand() % 100) / 100.0;
bool sprayBrush = false;


int SIZE = 1;
float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3]; // width, height, rgb -> will use this to store the pixels
bool mask[WINDOW_HEIGHT][WINDOW_WIDTH];
GLFWwindow *window;

// Color structure. Can be used to define the brush and background color.
struct color
{
	float r, g, b;
	color(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

// starting draw color is white
color startingColor = color(1, 1, 1);
// starting background color is black
color backgroundColor = color(0, 0, 0);
// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner.
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);
	// std::cout << "x: " << x << " y: " << y << std::endl;
	mask[y][x] = true;
	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;
}

void ClearFrameBuffer() // will set the frame buffer to black
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}
void redo()
{ // i = row, j = column (y, x)
	for (int i = 0; i < 600; i++)
	{
		for (int j = 0; j < 900; j++)
		{
			if (mask[i][j])
			{
				mask[i][j] = false;
				frameBuffer[i][j][0] = backgroundColor.r;
				frameBuffer[i][j][1] = backgroundColor.g;
				frameBuffer[i][j][2] = backgroundColor.b;
			}
		}
	}
}
// Mouse position callback function: used to get position of the mouse
void CursorPositionCallback(GLFWwindow *lWindow, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
		int count = 0;
		if (!sprayBrush)
		{
			for (int i = 0; i < SIZE; i++)
			{
				for (int j = 0; j < SIZE; j++)
				{
					// count++;
					// std::cout << count << std::endl;
					if (circleBrush)
					{ // draw circle
						int x1 = xpos + i;
						int y1 = ypos + j;

						int distance = sqrt(pow(x1 - xpos, 2) + pow(y1 - ypos, 2));
						if (distance <= SIZE)
						{
							SetFrameBufferPixel(xpos + i, ypos + j, startingColor);
						}
						int x2 = xpos - i;
						int y2 = ypos - j;

						distance = sqrt(pow(x2 - xpos, 2) + pow(y2 - ypos, 2));
						if (distance <= SIZE)
						{
							SetFrameBufferPixel(xpos - i, ypos - j, startingColor);
						}

						int x3 = xpos + i;
						int y3 = ypos - j;

						distance = sqrt(pow(x3 - xpos, 2) + pow(y3 - ypos, 2));
						if (distance <= SIZE)
						{
							SetFrameBufferPixel(xpos + i, ypos - j, startingColor);
						}

						int x4 = xpos - i;
						int y4 = ypos + j;

						distance = sqrt(pow(x4 - xpos, 2) + pow(y4 - ypos, 2));
						if (distance <= SIZE)
						{
							SetFrameBufferPixel(xpos - i, ypos + j, startingColor);
						}
					}
					else
					{ // rect
						SetFrameBufferPixel(xpos + i, ypos + j, startingColor);
						SetFrameBufferPixel(xpos - i, ypos - j, startingColor);
						SetFrameBufferPixel(xpos + i, ypos - j, startingColor);
						SetFrameBufferPixel(xpos - i, ypos + j, startingColor);
					}
				}
			}
		}
		else // paint brush
		{
			int totalPixels = (SIZE + 1) * (SIZE + 1);
			int count = 0;
			double percentageDrawn = 0.0;
			while (percentageDrawn < percentageToDraw)
			{
				for (int i = 0; i < SIZE; i++)
				{
					for (int j = 0; j < SIZE; j++)
					{
						if (rand() % 50 < 25)
						{
							SetFrameBufferPixel(xpos + i, ypos + j, startingColor);
							SetFrameBufferPixel(xpos - i, ypos - j, startingColor);
							SetFrameBufferPixel(xpos + i, ypos - j, startingColor);
							SetFrameBufferPixel(xpos - i, ypos + j, startingColor);
							count += 4;
							percentageDrawn = (double) count / (double) totalPixels;
							if (percentageDrawn > percentageToDraw)
							{
								break;
							}
						}
					}
				}
			}
		}
	}
}

// Mouse callback function: used to handle mouse clicks
void MouseCallback(GLFWwindow *lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Mouse left button is pressed." << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		std::cout << "Mouse right button is pressed." << std::endl;
		redo();
	}
}
void colorBackground()
{
	for (int i = 0; i < 600; i++)
	{
		for (int j = 0; j < 900; j++)
		{
			if (!mask[i][j])
			{
				frameBuffer[i][j][0] = backgroundColor.r;
				frameBuffer[i][j][1] = backgroundColor.g;
				frameBuffer[i][j][2] = backgroundColor.b;
			}
		}
	}
}

// Keyboard callback function: called when a key is pressed
void CharacterCallback(GLFWwindow *lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	if (key == '+' && SIZE < 128)
	{
		SIZE *= 2;
		std::cout << "Size of brush was increased by factor of 2.." << std::endl;
		std::cout << "Size: " << SIZE << std::endl;
	}
	else if (key == '-' && SIZE > 1)
	{
		SIZE /= 2;
		std::cout << "Size of brush was decreased by factor of 2.." << std::endl;
		std::cout << "Size: " << SIZE << std::endl;
	}
	else if (key == '0')
	{
		startingColor = color(0, 0, 0);
	}
	else if (key == '1')
	{
		startingColor = color(0, 0, 1);
	}
	else if (key == '2')
	{
		startingColor = color(0, 1, 0);
	}
	else if (key == '3')
	{
		startingColor = color(0, 1, 1);
	}
	else if (key == '4')
	{
		startingColor = color(1, 0, 0);
	}
	else if (key == '5')
	{
		startingColor = color(1, 0, 1);
	}
	else if (key == '6')
	{
		startingColor = color(1, 1, 0);
	}
	else if (key == '7')
	{
		startingColor = color(1, 1, 1);
	}
	else if (key == ')')
	{
		backgroundColor = color(0, 0, 0);
		colorBackground();
	}
	else if (key == '!')
	{
		backgroundColor = color(0, 0, 1);
		colorBackground();
	}
	else if (key == '@')
	{
		backgroundColor = color(0, 1, 0);
		colorBackground();
	}
	else if (key == '#')
	{
		backgroundColor = color(0, 1, 1);
		colorBackground();
	}
	else if (key == '$')
	{
		backgroundColor = color(1, 0, 0);
		colorBackground();
	}
	else if (key == '%')
	{
		backgroundColor = color(1, 0, 1);
		colorBackground();
	}
	else if (key == '^')
	{
		backgroundColor = color(1, 1, 0);
		colorBackground();
	}
	else if (key == '&')
	{
		backgroundColor = color(1, 1, 1);
		colorBackground();
	}
	// implementing change between rectangular brush and circle brush
	else if (key == 'b')
	{
		if (rectBrush)
		{
			rectBrush = false;
			circleBrush = true;
			std::cout << "circle brush enabled\n";
		}
		else
		{
			circleBrush = false;
			rectBrush = true;
			std::cout << "rectangular brush enabled\n";
		}
	}
	else if (key == 's')
	{
		if (!sprayBrush)
		{
			sprayBrush = true;
			std::cout << "Spray brush enabled" << std::endl;
		}
		else
		{
			sprayBrush = false;
			std::cout << "Spray brush disabled" << std::endl;
		}
	}
}

void Init()
{
	glfwInit();
	// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - Casey Quinn", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
}

int main()
{
	Init(); // initialize the window and OpenGL context
	while (glfwWindowShouldClose(window) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display(); // display the frame buffer
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents(); // check for events to see if any updates from the inputs
	}

	glfwTerminate();
	return 0;
}