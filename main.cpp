
#include "Objects.h"
#include <GL\glui.h>

/**************************************** myGlutKeyboard() **********/

void Keyboard(unsigned char Key, int x, int y)
{
    switch (Key)
    {
    case 27:
    case 'q':
    case 'Q':
        exit(0);
        break;
    }

    glutPostRedisplay();
}

/**************************************** mySpecialKey() *************/

static void SpecialKey(int key, int x, int y)
{
    printf("Tecla especial presionada: %d, Seleccion actual: %d\n", key, escena.seleccion);
    
    TPrimitiva *car = escena.GetCar(escena.seleccion);
    float giro = 4.0f;
    float despl = 1.5f;
    float girocoef = 5.0f;

    // pequeño factor para que girar con las flechas afecte la orientacion
    float steerImmediateFactor = 8.0f; // divider: mayor -> efecto inmediato menor

    // Comprobar que el puntero car no sea NULL
    if (car == NULL) {
        printf("ERROR: No hay coche seleccionado (car == NULL)\n");
        return;
    }
    
    printf("Coche ID %d encontrado en posicion (%.2f, %.2f, %.2f)\n", car->ID, car->tx, car->ty, car->tz);

    switch (key)
    {
    case GLUT_KEY_UP: // El coche avanza
        car->rr += 8;

        car->gc += (car->gr / (girocoef * 2.0f)); // Cambia orientación según ángulo de ruedas
        car->tx -= despl * glm::sin(glm::radians(car->gc));
        car->ty += despl * glm::cos(glm::radians(car->gc));
        printf("ARRIBA: Nueva pos (%.2f, %.2f) heading=%.2f gr=%.2f\n", car->tx, car->ty, car->gc, car->gr);
        break;

    case GLUT_KEY_DOWN: // El coche retrocede
        car->rr -= 8;

        car->gc -= (car->gr / (girocoef * 2.0f)); // Retrocede girando en sentido opuesto
        car->tx += despl * glm::sin(glm::radians(car->gc));
        car->ty -= despl * glm::cos(glm::radians(car->gc));
        printf("ABAJO: Nueva pos (%.2f, %.2f) heading=%.2f gr=%.2f\n", car->tx, car->ty, car->gc, car->gr);
        break;

    case GLUT_KEY_LEFT: // Girar a la izquierda
        car->gr += giro;
        if (car->gr > 45.0f)
            car->gr = 45.0f;
        printf("IZQUIERDA: gr=%.2f\n", car->gr);
        break;

    case GLUT_KEY_RIGHT: // Girar a la derecha
        car->gr -= giro;
        if (car->gr < -45.0f)
            car->gr = -45.0f;
        printf("DERECHA: gr=%.2f\n", car->gr);
        break;
    }
    glutPostRedisplay();
}

/***************************************** myGlutMenu() ***********/

void Menu(int value)
{
    Keyboard(value, 0, 0);
}

static bool left_dragging = false;
static int last_x = 0;
static float mouse_sensitivity = 1.0f;

void Mouse(int button, int button_state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(button_state == GLUT_DOWN)
        {
            left_dragging = true;
            last_x = x;
        }
        else if(button_state == GLUT_UP)
        {
            left_dragging = false;
        }
    }

    gui.Mouse(button, button_state, x, y);
}

void Render()
{
    escena.Render();
}

void Idle()
{
    gui.Idle();
}

void Reshape(int x, int y)
{
    gui.Reshape(x, y);
}

void Motion(int x, int y)
{
    if(left_dragging && escena.camara == 0) // Modo cámara libre
    {
        int dx = x - last_x;
        escena.view_position[0] += dx * (mouse_sensitivity * 0.01f);
        last_x = x;
        glutPostRedisplay();
    }
    gui.Motion(x, y);
}

/**************************************** main() ********************/

int main(int argc, char *argv[])
{
    // Inicializa GLUT and crea la ventana principal
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(1200, 800);

    int main_window = glutCreateWindow("CarGL V2.2 (2022) con Shaders");

    // Inicializa los valores de OpenGL para esta Aplicaci�n
    escena.InitGL();
    gui.Init(main_window);

    glutDisplayFunc(Render);
    GLUI_Master.set_glutReshapeFunc(Reshape);
    GLUI_Master.set_glutKeyboardFunc(Keyboard);
    GLUI_Master.set_glutSpecialFunc(SpecialKey);
    GLUI_Master.set_glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc(Idle);

    // Crea el escenario
    escena.CrearEscenario();

    /**** Regular GLUT main loop ****/
    glutMainLoop();

    return EXIT_SUCCESS;
}
