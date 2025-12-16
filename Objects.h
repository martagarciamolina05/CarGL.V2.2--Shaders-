
/***************************************************************************************

    CarGL (Objects.h)

    Grado en Ingenier�a Multimedia.
    Pr�ctica de Gr�ficos por Computador.
    OpenGL con Shaders.
  --------------------------------------------------------------------------------------

    2012 - 2022 (C) Juan Antonio Puchol Garc�a (puchol@dccia.ua.es)

****************************************************************************************/

//---------------------------------------------------------------------------
#ifndef ObjectsH
#define ObjectsH
//---------------------------------------------------------------------------

#define GLEW_STATIC

#include "Shader.h"
#include "Program.h"

#include <GL/glui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Identificadores internos de los objetos y el coche
#define CARRETERA_ID    10
#define EDIFICIO1_ID     20
#define EDIFICIO2_ID     21
#define EDIFICIO3_ID     22

#define COCHE_ID	    500

#define BANCO_ID 5
#define VALLA_ID 15
#define FUENTE_ID 11
#define ARB_FUENTE_ID 12
#define FAROLA_ID 13
#define BASURA_ID 14

#define CAMPO_ID 30

// IDs para los callbacks de TGui
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT2_ENABLED_ID    202
#define LIGHT0_POSITION_ID   210
#define LIGHT1_POSITION_ID   211
#define LIGHT2_POSITION_ID   212
#define LIGHT0_INTENSITY_ID  220
#define LIGHT1_INTENSITY_ID  221
#define LIGHT2_INTENSITY_ID  222

#define SPECULAR_INTENSITY_ID 230
#define SHININESS_ID          231

#define ENABLE_ID            300
#define DISABLE_ID           301

#define RESET_ID             400

#define SEL_ID               500

#define CAMARA_ID            600
#define PROYECCION_ID        610
#define WINDING_ID           620
#define MODO_VIS_ID          630
#define SOMBREADO_ID         640

// Datos del formato 3DS (x, y, z, A, B, C, u, v)
#define POSITION_COMPONENT_COUNT    3
#define NORMAL_COMPONENT_COUNT      3
#define UV_COMPONENT_COUNT          2
// C�lculo del stride (3+3+2)*4 = 8*4 = 32
#define STRIDE                      32

// Nombre de los attributes
#define A_POSITION  "a_Position"
#define A_NORMAL    "a_Normal"
#define A_UV        "a_UV"

// Nombre de los uniforms
#define U_PROJECTIONMATRIX      "u_ProjectionMatrix"
#define U_MVMATRIX              "u_MVMatrix"
#define U_VMATRIX               "u_VMatrix"
#define U_COLOR                 "u_Color"
#define U_LUZ0                  "u_Luz0"
#define U_LUZ1                  "u_Luz1"
#define U_LUZ2                  "u_Luz2"

#define U_LUZ0POS               "u_Light0Pos"
#define U_DIFFUSEI0              "u_DiffuseI0"
#define U_LUZ1POS               "u_Light1Pos"
#define U_DIFFUSEI1              "u_DiffuseI1"
#define U_LUZ2POS               "u_Light2Pos"
#define U_DIFFUSEI2              "u_DiffuseI2"

#define U_TEXTURE               "u_Texture"
#define U_USETEXTURE            "u_UseTexture"
#define U_RETILING              "u_Retiling"
#define U_SPECULARINTENSITY     "u_SpecularIntensity"
#define U_SHININESS             "u_Shininess"
#define U_SHADINGMODE           "u_ShadingMode"

//************************************************************** Clase TPrimtiva

class TPrimitiva
{
public: // Atributos de la clase
		int ID;				    // DisplayLists del objeto
		int tipo;               // Tipo de Objeto
		float tx,ty,tz; 	    // Posicion del objeto
		float sx,sy,sz; 	    // Escalado del objeto
		float rx,ry,rz;     	// Rotacion del objeto
		float rr;               // Rotacion de las ruedas
		float colores[3][4];    // Color RGB y canal Alfa

		float   *modelo0;        // modelo a representar
		int     num_vertices0;   // número de vértices
        float   *modelo1;        // modelo a representar
		int     num_vertices1;   // número de vértices

		float   *modelo2;        // modelo a representar
		int     num_vertices2;

		float   *modelo3;        // modelo a representar
		int     num_vertices3;

		float   *modelo4;        // modelo a representar
		int     num_vertices4;

		GLuint  textureID;       // ID de la textura OpenGL

		float gc,gr,d;


public: // M�todos
 		TPrimitiva(int DL, int tipo);
        void __fastcall Render(int seleccion, bool reflejo=false);
};

//************************************************************** Clase TEscena

class TEscena
{
public: // Atributos de la clase
		int   	seleccion;   	// Objeto seleccionado, 0=ninguno
        int		num_objects;    // N�mero de objetos (excepto coches)
        int     num_cars;       // N�mero de coches


        int camara;

        TPrimitiva  *cars[10];
        TPrimitiva  *objects[100];

        // Handles de los attributes y uniforms
        int aPositionLocation;
        int aNormalLocation;
        int aUVLocation;
        int uProjectionMatrixLocation;
        int uMVMatrixLocation;
		int uVMatrixLocation;
		int uColorLocation;
		int uLuz0Location;

        int uLuz0PosLocation;
        int uDifusseI0Location;

		int uLuz1Location;
        int uLuz1PosLocation;
        int uDifusseI1Location;

		int uLuz2Location;
        int uLuz2PosLocation;
        int uDifusseI2Location;

		int uTextureLocation;    // Uniform para la textura
		int uUseTextureLocation; // Uniform para indicar si usa textura
		int uRetilingLocation;   // Uniform para repetición de textura
		int uAmbientLocation;    // Uniform para luz ambiente
        int uSpecularLocation; // Uniform para luz especular
        int uShininessLocation; // Uniform para brillo especular
        int uShadingModeLocation; // Uniform para modo de sombreado
    

		glm::mat4 projectionMatrix; // Almacena la matriz de proyecci�n
        glm::mat4 viewMatrix;
        glm::mat4 viewBaseMatrix = glm::mat4(1.0f);      // Almacena la matriz de la vista (c�mara)

		Program  *shaderProgram;    // Almacena el programa de OpenGL (ShaderProgram)

        // Vectores de luces y materiales
        GLfloat light0_ambient[4];
        GLfloat light0_diffuse[4];
        GLfloat light0_specular[4];
        GLfloat light0_position[4];

        GLfloat light1_ambient[4];
        GLfloat light1_diffuse[4];
        GLfloat light1_specular[4];
        GLfloat light1_position[4];

        GLfloat light2_ambient[4];
        GLfloat light2_diffuse[4];
        GLfloat light2_specular[4];
        GLfloat light2_position[4];

        GLfloat mat_ambient[4];
        GLfloat mat_diffuse[4];
        GLfloat mat_specular[4];
        GLfloat mat_shininess[1];

        float   xy_aspect;
        int     last_x, last_y;

        
        int     wireframe;
        int     z_buffer;
        int     culling;
        int     winding_order;  // 0=CCW (antihorario), 1=CW (horario)
        int     shader;         // 0=plano, 1=Gouraud (por vértice), 2=Phong (por fragmento)

        int     show_car;
        int     show_wheels;
        int     show_road;
        int     proyeccion;

        GLfloat view_position[3];
        GLfloat view_rotate[16];
        float   scale;
        float  scaleX;
        float  scaleY;
        float  scaleZ;
        float   ambient_intensity;  // Intensidad de luz ambiente

public: // M�todos
		TEscena();

        void __fastcall InitGL();
		void __fastcall Render();
		void __fastcall RenderCars(bool reflejo=false);
		void __fastcall RenderObjects(bool reflejo=false);

		void __fastcall AddCar(TPrimitiva *car);
		void __fastcall AddObject(TPrimitiva *object);
        void __fastcall CrearEscenario();

		TPrimitiva __fastcall *GetCar(int id);

		void __fastcall Pick3D(int mouse_x, int mouse_y);

        void sendLights();


};

//************************************************************** Clase TGui

class TGui //clase de la interfaz
{
public:
        int             window_id;

        // live variables usadas por GLUI
        int             sel;
        int             sel_camara;
        int             enable_panel2;
        int             light0_enabled;
        int             light1_enabled;
        int             light2_enabled;
        float           light0_intensity;
        float           light1_intensity;
        float           light2_intensity;
        float           light0_position[4];
        float           light1_position[4];
        float           light2_position[4];
        float           specular_intensity;
        float           shininess;

        GLUI            *glui, *glui2;
        GLUI_Spinner    *light0_spinner;
        GLUI_Spinner    *light1_spinner;
        GLUI_Spinner    *light2_spinner;
        GLUI_RadioGroup *radio;
        GLUI_Panel      *obj_panel;
        GLUI_Rotation   *view_rot;

public:
        TGui();
        void __fastcall Init(int main_window);
        void __fastcall ControlCallback(int control);
        void __fastcall Idle( void );
        void __fastcall Reshape( int x, int y  );
        void __fastcall Motion( int x, int y  );
        void __fastcall Mouse(int button, int button_state, int x, int y );

        void cambiarPosCamara();
};

//************************************************************** Variables de clase

extern TEscena  escena;
extern TGui     gui;

#endif
