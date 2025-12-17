
/****************************************************************************************

    CarGL (Objects.cpp)

    Grado en Ingenier�a Multimedia.
    Pr�ctica de Gr�ficos por Computador.
    OpenGL con Shaders.
  ---------------------------------------------------------------------------------------

    2012 - 2022 (C) Juan Antonio Puchol Garc�a (puchol@dccia.ua.es)


*****************************************************************************************/

#include "Objects.h"
#include <GL/glui.h>

#include "load3ds.c"
#include "loadjpeg.h"
#include "loadjpeg.c"

// Variable para inicializar los vectores correpondientes con los valores iniciales
GLfloat light0_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light0_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light0_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light0_position_c[4] = {-100.0f, 100.0f, 50.0f, 1.0f };

GLfloat light1_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light1_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light1_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light1_position_c[4] = {   0.0f, 100.0f,  0.0f, 1.0f };

GLfloat light2_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light2_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light2_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light2_position_c[4] = { 100.0f, 100.0f,  0.0f, 1.0f };

GLfloat mat_ambient_c[4]    = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat mat_diffuse_c[4]    = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat mat_specular_c[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mat_shininess_c[1] = { 100.0f };

// Matriz de 4x4 = (I)
float view_rotate_c[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_position_c[3] = { 0.0, -2.0, -9.0 };

float coloresc_c[2][4] = { {0.8, 0.5, 0.0, 1.0}, {0.5, 0.5, 0.5, 1.0}}; // Color del coche
float coloresr_c[3][4] = { {0.3, 0.3, 0.3, 1.0}, {1.0, 1.0, 1.0, 1.0},{0.2, 0.5, 0.2, 1.0}}; // Color de la carretera

//************************************************************** Variables de clase

TEscena escena;
TGui    gui;

//************************************************************** Clase TPrimitiva

TPrimitiva::TPrimitiva(int DL, int t)
{

    ID   = DL;
    tipo = t;

    sx = sy = sz = 1;
    rx = ry = rz = 0;

    // Crear textura blanca por defecto (1x1 pixel blanco)
    unsigned char whitePixel[] = {255, 255, 255};
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	switch (tipo) {
		case CARRETERA_ID: {  // Creaci�n de la carretera
		    tx = tz = 0;
		    ty = -10;

            memcpy(colores, coloresr_c, 12*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/suelo2.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/lineasCarretera.3ds", &num_vertices1);
            modelo2 = Load3DS("../../Modelos/suelo.3ds", &num_vertices2);

            break;
		}
		case CAMPO_ID: {  // Creaci�n de la carretera
		    tx  = 0;
		    ty = 0;
		    tz = 5;

            memcpy(colores, coloresr_c, 12*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/sueloCampo.3ds", &num_vertices0);


             //************************ Cargar textura JPEG **********************************
            int width, height;
            unsigned char *pixels = LoadJPEG("../../Modelos/texturas/cespedTextura.jpeg", &width, &height);

            if (pixels != NULL) {
                printf("Textura Campo cargada: %dx%d\n", width, height);
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                printf("glTexImage2D ejecutado OK\n");
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // Configurar wrapping para que se repita correctamente
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                printf("Generando mipmaps...\n");
                glGenerateMipmap(GL_TEXTURE_2D);
                printf("Mipmaps generados OK\n");
                free (pixels);
            } else {
                printf("ERROR: No se pudo cargar cesped.jpeg\n");
            }

            printf("Cargando lineasCampo.3ds...\n");
            modelo1 = Load3DS("../../Modelos/lineasCampo.3ds", &num_vertices1);
            printf("Cargando vallasCampo.3ds...\n");
            modelo2 = Load3DS("../../Modelos/vallasCampo.3ds", &num_vertices2);
            printf("Modelos del campo cargados\n");


            break;
		}
		case EDIFICIO1_ID: {  // Creaci�n de la carretera
		    tz = ty = 0;
            tx = -27;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/edificio1.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case EDIFICIO2_ID: {  // Creaci�n de la carretera
		    tz = ty = 0;
            tx = -27;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/edificio2.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case EDIFICIO3_ID: {  // Creaci�n de la carretera
		    tz = tx = 0;
            ty = -5;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/edificio3.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case VALLA_ID: {  // Creaci�n de la carretera
		    tz = ty = 0;
            tx = -40;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/valla.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case FUENTE_ID: {  // Creaci�n de la carretera
		    tz = ty = tx=0;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/fuente.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/aguaFuente.3ds", &num_vertices1);
            break;
		}
		case ARB_FUENTE_ID: {  // Creaci�n de la carretera
		    tz = ty = tx=0;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/arbusto.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case BANCO_ID: {  // Creaci�n de la carretera
		    tz = 5;
            ty = -5;
            tx=5;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/bancoDef.3ds", &num_vertices0);

            //************************ Cargar textura JPEG **********************************
            int width, height;
            unsigned char *pixels = LoadJPEG("../../Modelos/texturas/textura_banco.jpeg", &width, &height);

            if (pixels != NULL) {

                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glGenerateMipmap(GL_TEXTURE_2D);
                free (pixels);
            } else {
                printf("ERROR: No se pudo cargar textura_banco.jpeg\n");
                // Mantiene la textura blanca por defecto creada en el constructor
            }
            modelo1 = NULL;
            break;
		}
		case BASURA_ID: {  // Creaci�n de la carretera
		    tz = 5;
            ty = -30;
            tx=0;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/basura.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case FAROLA_ID: {  // Creaci�n de la carretera
		    tz = 5;
            ty = -5;
            tx=5;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/farola.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case COCHE_ID: { // Creaci�n del coche

		    tx = 0.0;
		    ty =  0.0;
		    tz =  0.0;
		    rr =  0.0;

		    //inicializar las variables
		    gc = 0.0;
		    gr = 0.0;

		    memcpy(colores, coloresc_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por v�rtice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/coche.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/rueda1.3ds", &num_vertices1);
            break;
		}
	} // switch
}

void __fastcall TPrimitiva::Render(int seleccion, bool reflejo)
{
    glm::mat4   modelMatrix;
    glm::mat4   modelViewMatrix;

    glStencilFunc(GL_ALWAYS, this->ID, 0xFF);

    // Activar la textura del objeto (por defecto es blanca, o la textura cargada)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(escena.uTextureLocation, 0);

    switch (tipo) {

        case CARRETERA_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera (asfalto)
                glUniform1i(escena.uUseTextureLocation, 0);
                
                // Material ASFALTO (sin brillo especular)
                glUniform4f(escena.uColorLocation, 0.15f, 0.15f, 0.15f, 1.0f); // Gris muy oscuro
                glUniform3f(escena.uSpecularColorLocation, 0.05f, 0.05f, 0.05f); 
                glUniform1f(escena.uSpecularLocation, 0.0f); // Sin brillo especular
                glUniform1f(escena.uShininessLocation, 1.0f); 
                
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
                
                // Restaurar valores por defecto
                glUniform3f(escena.uSpecularColorLocation, 1.0f, 1.0f, 1.0f);
                glUniform1f(escena.uSpecularLocation, gui.specular_intensity);
                glUniform1f(escena.uShininessLocation, gui.shininess);

                // Pintar las l�neas
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[1]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);


                // Pintar el césped
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[2]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo2);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo2+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices2);


            }
            break;
        } // case CARRETERA_ID:
        case EDIFICIO1_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case CAMPO_ID: {
            if (escena.show_road) {
                // Calculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Envia nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar el campo con textura
                glUniform1i(escena.uUseTextureLocation, 1); // Activar textura
                glUniform1f(escena.uRetilingLocation, 6.0f); // Repetir textura 6 veces
                glUniform4f(escena.uColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
                //
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Asociar el buffer de coordenadas UV
                glVertexAttribPointer(escena.aUVLocation, 2, GL_FLOAT, GL_FALSE, STRIDE, modelo0 + 6);
                glEnableVertexAttribArray(escena.aUVLocation);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

                // Deshabilitar coordenadas UV para los siguientes modelos
                glDisableVertexAttribArray(escena.aUVLocation);

                // Pintar las líneas del campo en blanco
                glUniform1i(escena.uUseTextureLocation, 0); // Desactivar textura
                glUniform4f(escena.uColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
                //
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);


                // Pintar las vallas en blanco
                glUniform4f(escena.uColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo2);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo2+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices2);


            }
            break;
        }
        case EDIFICIO2_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case EDIFICIO3_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rz), glm::vec3(0,0,1)); // Rot Z

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case VALLA_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case BANCO_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rz), glm::vec3(0,0,1)); // Rot Z


                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 1);
                glUniform1f(escena.uRetilingLocation, 1.0f); // Sin repetición de textura
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // Asociar el buffer de coordenadas UV
                glVertexAttribPointer(escena.aUVLocation, 2, GL_FLOAT, GL_FALSE, STRIDE, modelo0 + 6);
                glEnableVertexAttribArray(escena.aUVLocation);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case FAROLA_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rz), glm::vec3(0,0,1)); // Rot Z


                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case BASURA_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rz), glm::vec3(0,0,1)); // Rot Z


                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }
        case FUENTE_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la fuente
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

                // Pintar el agua si existe
                if (modelo1 != NULL && num_vertices1 > 0) {
                    // Material AGUA: azul translúcido con brillo especular alto
                    glUniform4f(escena.uColorLocation, 0.2f, 0.4f, 0.7f, 0.7f); 
                    glUniform3f(escena.uSpecularColorLocation, 1.0f, 1.0f, 1.0f);
                    glUniform1f(escena.uSpecularLocation, 1.5f); // Muy brillante
                    glUniform1f(escena.uShininessLocation, 128.0f);
                    
                    glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                    glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);
                    glDrawArrays(GL_TRIANGLES, 0, num_vertices1);
                    
                    // Restaurar valores por defecto
                    glUniform3f(escena.uSpecularColorLocation, 1.0f, 1.0f, 1.0f);
                    glUniform1f(escena.uSpecularLocation, gui.specular_intensity);
                    glUniform1f(escena.uShininessLocation, gui.shininess);
                }
            }
            break;
        }
        case ARB_FUENTE_ID: {
            if (escena.show_road) {
                // C�lculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad

                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotaci�n alrededor del eje y en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                //                   Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }

        case COCHE_ID: {
            if (escena.show_car) {
                glUniform1i(escena.uUseTextureLocation, 0);
                
                // Aplicar material según el ID del coche
                if (this->ID == 1) {

                    //COCHE 1 : ROJO (METAL)
                    glUniform4f(escena.uColorLocation, 0.6f, 0.1f, 0.1f, 1.0f);
                    glUniform3f(escena.uSpecularColorLocation, 0.75f, 0.6f, 0.6f);
                    glUniform1f(escena.uSpecularLocation, 0.9f);
                    glUniform1f(escena.uShininessLocation, 90.0f);
                } else if (this->ID == 2) {

                    // COCHE 2 : AZUL (METAL)
                    glUniform4f(escena.uColorLocation, 0.1f, 0.2f, 0.7f, 1.0f);
                    glUniform3f(escena.uSpecularColorLocation, 0.6f, 0.65f, 0.8f);
                    glUniform1f(escena.uSpecularLocation, 0.9f);
                    glUniform1f(escena.uShininessLocation, 90.0f);
                }
                
                // Asociamos los v�rtices y sus normales
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                // C�lculo de la matriz modelo
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::translate(modelMatrix,glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(gc), glm::vec3(0,0,1));
                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Env�a nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]); //si esta sel, se pinta de blanco


                if(this->ID == seleccion){
                    glUniform4f(escena.uColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
                    glUniform3f(escena.uSpecularColorLocation, 1.0f, 1.0f, 1.0f);
                    glUniform1f(escena.uSpecularLocation, gui.specular_intensity);
                    glUniform1f(escena.uShininessLocation, gui.shininess);
                }
                // Si no está seleccionado, mantener el material plata ya configurado

                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }


            if (escena.show_wheels)
            {
                // Distancias desde el CENTRO del coche a cada rueda (en el sistema local del coche)
                float mitad_ancho = 14.0f;          // Separación lateral izq/der desde centro
                float dist_delante = 25.0f;         // Distancia del centro al frente
                float dist_atras = -19.5f;          // Distancia del centro hacia atrás (negativa)
                float altura_rueda = -14.0f;        // Offset Z para bajar ruedas al suelo

                float rad_gc = glm::radians(gc);

                glUniform1i(escena.uUseTextureLocation, 0);
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[1]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);

                // RUEDA Delantera Izquierda (posición local: adelante, a la izquierda)
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));           // Posición del coche
                modelMatrix = glm::rotate(modelMatrix, rad_gc, glm::vec3(0,0,1));          // Rotación del coche
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-mitad_ancho, dist_delante, altura_rueda)); // Offset local
                modelMatrix = glm::rotate(modelMatrix, glm::radians(gr), glm::vec3(0,0,1)); // Dirección rueda
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rr), glm::vec3(1,0,0)); // Rotación rodadura
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Delantera Derecha (posición local: adelante, a la derecha)
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix = glm::rotate(modelMatrix, rad_gc, glm::vec3(0,0,1));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(mitad_ancho, dist_delante, altura_rueda));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(gr), glm::vec3(0,0,1));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rr), glm::vec3(1,0,0));
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Trasera Izquierda (posición local: atrás, a la izquierda)
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix = glm::rotate(modelMatrix, rad_gc, glm::vec3(0,0,1));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-mitad_ancho, dist_atras, altura_rueda));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rr), glm::vec3(1,0,0));
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);

                // RUEDA Trasera Derecha (posición local: atrás, a la derecha)
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix = glm::rotate(modelMatrix, rad_gc, glm::vec3(0,0,1));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(mitad_ancho, dist_atras, altura_rueda));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rr), glm::vec3(1,0,0));
                modelViewMatrix = escena.viewMatrix * modelMatrix;
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);
            }
            break;
        } // case COCHE_ID:
    } // switch

}

//************************************************************** Clase TEscena

TEscena::TEscena() {

    seleccion = 1;
    num_objects = 0;
    num_cars = 0;

    show_car = 1;
    show_wheels = 1;
    show_road = 1;

    // live variables usadas por GLUI en TGui
    wireframe = 0;
    z_buffer = 1;
    culling = 0;
    winding_order = 0;  // CCW por defecto
    proyeccion = 0;
    shader = 2;  // Phong (por fragmento) por defecto

    scale = 100.0;
    scaleX = 100.0;
    scaleY = 100.0;
    scaleZ = 100.0;
    xy_aspect = 1;
    last_x = 0;
    last_y = 0;
    ambient_intensity = 0.15f;  // 15% por defecto

    memcpy(view_position, view_position_c, 3*sizeof(float));
    memcpy(view_rotate, view_rotate_c, 16*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));
    memcpy(light0_diffuse, light0_diffuse_c, 4*sizeof(float));
    memcpy(light0_specular, light0_specular_c, 4*sizeof(float));
    memcpy(light0_position, light0_position_c, 4*sizeof(float));

    memcpy(light1_ambient, light1_ambient_c, 4*sizeof(float));
    memcpy(light1_diffuse, light1_diffuse_c, 4*sizeof(float));
    memcpy(light1_specular, light1_specular_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));

    memcpy(light2_ambient, light2_ambient_c, 4*sizeof(float));
    memcpy(light2_diffuse, light2_diffuse_c, 4*sizeof(float));
    memcpy(light2_specular, light2_specular_c, 4*sizeof(float));
    memcpy(light2_position, light2_position_c, 4*sizeof(float));

    memcpy(mat_ambient, mat_ambient_c, 4*sizeof(float));
    memcpy(mat_diffuse, mat_diffuse_c, 4*sizeof(float));
    memcpy(mat_specular, mat_specular_c, 4*sizeof(float));
    memcpy(mat_shininess, mat_shininess_c, 1*sizeof(float));
}

void __fastcall TEscena::InitGL()
{
    int tx, ty, tw, th;

    // Habilita el z_buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    //PARA ACTIVAR EL BUFFER DE STENCIL
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


    // Inicializaci�n de GLEW
    std::cout << "Inicializando GLEW" << std::endl << std::endl;
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendedor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Carga de los Shaders
    std::cout << std::endl << "Cargando Shaders" << std::endl;

    Shader shader;

    std::vector<GLuint> shaders;
    shaders.push_back(shader.LoadShader("../../Shaders/VertexShader.glsl", GL_VERTEX_SHADER));
    //std::cout << "Vertex Shader: " << shader.ReturnShaderID() << std::endl;
    shaders.push_back(shader.LoadShader("../../Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER));
    //std::cout << "Fragment Shader: " << shader.ReturnShaderID() << std::endl;
    shaderProgram = new Program(shaders);

    //std::cout << "Shader Program: " << shaderProgram->ReturnProgramID() << std::endl;

    glUseProgram(shaderProgram->ReturnProgramID());
    //glValidateProgram(shaderProgram->ReturnProgramID());

    aPositionLocation=shaderProgram->attrib(A_POSITION);
    aNormalLocation=shaderProgram->attrib(A_NORMAL);
    aUVLocation=shaderProgram->attrib(A_UV);

    uProjectionMatrixLocation=shaderProgram->uniform(U_PROJECTIONMATRIX);
    uMVMatrixLocation=shaderProgram->uniform(U_MVMATRIX);
    uVMatrixLocation=shaderProgram->uniform(U_VMATRIX);
    uColorLocation=shaderProgram->uniform(U_COLOR);
    uLuz0Location=shaderProgram->uniform(U_LUZ0);
    // Obtener las ubicaciones de las uniforms usadas por la luz 0
    uLuz0PosLocation = shaderProgram->uniform(U_LUZ0POS);
    uDifusseI0Location = shaderProgram->uniform(U_DIFFUSEI0);

    // Obtener las ubicaciones de las uniforms usadas por la luz 1
    uLuz1Location = shaderProgram->uniform(U_LUZ1);
    uLuz1PosLocation = shaderProgram->uniform(U_LUZ1POS);
    uDifusseI1Location = shaderProgram->uniform(U_DIFFUSEI1);

    // Obtener las ubicaciones de las uniforms usadas por la luz 2
    uLuz2Location = shaderProgram->uniform(U_LUZ2);
    uLuz2PosLocation = shaderProgram->uniform(U_LUZ2POS);
    uDifusseI2Location = shaderProgram->uniform(U_DIFFUSEI2);

    // Obtener la ubicación del uniform de textura
    uTextureLocation = shaderProgram->uniform(U_TEXTURE);
    uUseTextureLocation = shaderProgram->uniform(U_USETEXTURE);
    uRetilingLocation = shaderProgram->uniform(U_RETILING);
    uAmbientLocation = shaderProgram->uniform("u_Ambient");
    
    // Obtener las ubicaciones de los uniforms especulares
    uSpecularLocation = shaderProgram->uniform(U_SPECULARINTENSITY);
    uShininessLocation = shaderProgram->uniform(U_SHININESS);
    uSpecularColorLocation = shaderProgram->uniform(U_SPECULARCOLOR);
    uShadingModeLocation = shaderProgram->uniform(U_SHADINGMODE);
    
    // Establecer valores por defecto para el material especular
    glUniform1f(uSpecularLocation, 0.1f);  // Intensidad especular baja (era 0.5, demasiado alto)
    glUniform1f(uShininessLocation, 64.0f); // Shininess alto para brillo más concentrado
    glUniform3f(uSpecularColorLocation, 1.0f, 1.0f, 1.0f); // Blanco por defecto
    glUniform1i(uShadingModeLocation, 2);   // Phong por defecto

    /*
    std::cout << "a_Position Location: " << aPositionLocation << std::endl;
    std::cout << "a_Normal Location: " << aNormalLocation << std::endl;

    std::cout << "u_ProjectionMatrix Location: " << uProjectionMatrixLocation << std::endl;
    std::cout << "u_MVMatrix Location: " << uMVMatrixLocation << std::endl;
    std::cout << "u_VMatrix Location: " << uVMatrixLocation << std::endl;
    std::cout << "u_Color Location: " << uColorLocation << std::endl;
    std::cout << "u_Luz0 Location: " << uLuz0Location << std::endl;
    */

    // Habilitamos el paso de attributes
    glEnableVertexAttribArray(aPositionLocation);
    glEnableVertexAttribArray(aNormalLocation);
    glEnableVertexAttribArray(aUVLocation);

    // Estableciendo la matriz de proyecci�n perspectiva
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    xy_aspect = (float)tw / (float)th;
    projectionMatrix = glm::perspective(45.0f, xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}


/************************** TEscena::AddCar(TPrimitiva *car) *****************/

void __fastcall TEscena::AddCar(TPrimitiva *car)
{
    cars[num_cars] = car;
    num_cars++;
}

/******************** TEscena::AddObject(TPrimitiva *object) *****************/

void __fastcall TEscena::AddObject(TPrimitiva *object)
{
    objects[num_objects] = object;
    num_objects++;
}

/******************** TPrimitiva *TEscena::GetCar(int id) ********************/

TPrimitiva __fastcall *TEscena::GetCar(int id)
{
    TPrimitiva *p=NULL;

    for (int i=0; i<num_cars; i++)
    {
        if (cars[i]->ID==id)
        {
            p = cars[i];
        }

    }
    return(p);
}

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderCars(bool reflejo) {

    for (int i=0; i<num_cars; i++)
    {
        cars[i]->Render(seleccion, reflejo);
    }
}

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderObjects(bool reflejo) {

    for (int i=0; i<num_objects; i++)
    {
        objects[i]->Render(seleccion, reflejo);
    }
}

/***************************************** TEscena::Render() *****************/

void __fastcall TEscena::Render()
{
    glm::mat4 rotateMatrix;

    glClearColor(0.0, 0.7, 0.9, 1.0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (z_buffer)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    if (culling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);  // Elimina las caras traseras
        
        // Configurar el sentido de las caras
        if (winding_order == 0)
            glFrontFace(GL_CCW);  // Antihorario (estándar)
        else
            glFrontFace(GL_CW);   // Horario
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    if(escena.proyeccion){
        projectionMatrix = glm::ortho(-6 * xy_aspect, 8 * xy_aspect, -3.f, 6.f, -100.f, 100.f);
    }else {
        projectionMatrix = glm::perspective(45.0f, xy_aspect, 0.1f, 1000.0f);
    }

    glUniformMatrix4fv(uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Modo de visualización: 0=sólido, 1=alámbrico, 2=puntos
    if(wireframe == 1)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //alambrico
    }
    else if(wireframe == 2)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //puntos
        glPointSize(3.0f); // Tamaño de los puntos
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //sólido
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



    if(camara== 1 || camara== 2){
        gui.cambiarPosCamara();
    }


    // C�lculo de la vista (c�mara)
    viewMatrix      = viewBaseMatrix;
    rotateMatrix    = glm::make_mat4(view_rotate);
    viewMatrix      = glm::translate(viewMatrix,glm::vec3(view_position[0], view_position[1], view_position[2]));
    viewMatrix      = viewMatrix*rotateMatrix;
    viewMatrix      = glm::scale(viewMatrix,glm::vec3((scale*scaleX)/10000.0, (scale*scaleY)/10000.0, (scale*scaleZ)/10000.0));

    //glUniform1i(uLuz0Location, gui.light0_enabled);

    this->sendLights();
    glUniform1f(uAmbientLocation, ambient_intensity);
    
    // Enviar los parámetros de brillo especular al shader
    glUniform1f(uSpecularLocation, gui.specular_intensity);
    glUniform1f(uShininessLocation, gui.shininess);
    glUniform3f(uSpecularColorLocation, 1.0f, 1.0f, 1.0f); // Blanco por defecto
    
    // Enviar el modo de sombreado actual (0=Flat, 1=Gouraud, 2=Phong)
    glUniform1i(uShadingModeLocation, shader);
    
    glUniformMatrix4fv(uVMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // Para la luz matrix view pero sin escalado!

    // Dibujar carretera y objetos
    RenderObjects(seleccion);

    // Dibujar coches
    RenderCars(seleccion);

    glutSwapBuffers();
}

// Selecciona un objeto a trav�s del rat�n
void __fastcall TEscena::Pick3D(int mouse_x, int mouse_y)
{
    GLuint index;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    int real_y = viewport[3] - mouse_y;

    glReadPixels(mouse_x, real_y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    printf("Click en mouse (%d, %d) -> OPENGL (%d, %d) || ID %d\n ", mouse_x, mouse_y, mouse_x, real_y, index);

    printf("Seleccion previo: %d\n", this->seleccion);

    this->seleccion = index;

    
    // 0 = NINGUNO, 1 = COCHE 1, 2 = COCHE 2
    if (this->seleccion == 1) {
        gui.sel = 1; // COCHE 1
    } else if (this->seleccion == 2) {
        gui.sel = 2; // COCHE 2
    } else {
        gui.sel = 0; // NINGUNO
    }
    
    gui.glui->sync_live();

}

// Crea todo el escenario
void __fastcall TEscena::CrearEscenario()
{
    printf("Creando carretera...\n");
    TPrimitiva *road = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);
    printf("Creando coches...\n");
    TPrimitiva *car = new TPrimitiva(1, COCHE_ID);
    TPrimitiva *car2 = new TPrimitiva(2, COCHE_ID);

    printf("Creando edificios...\n");
    TPrimitiva *edificio1 = new TPrimitiva(EDIFICIO1_ID, EDIFICIO1_ID);
    TPrimitiva *edificio2 = new TPrimitiva(EDIFICIO1_ID, EDIFICIO1_ID);
    TPrimitiva *edificio3 = new TPrimitiva(EDIFICIO2_ID, EDIFICIO2_ID);
    TPrimitiva *edificio4 = new TPrimitiva(EDIFICIO3_ID, EDIFICIO3_ID);
    TPrimitiva *edificio5 = new TPrimitiva(EDIFICIO3_ID, EDIFICIO3_ID);
    TPrimitiva *edificio6 = new TPrimitiva(EDIFICIO3_ID, EDIFICIO3_ID);
    TPrimitiva *edificio7 = new TPrimitiva(EDIFICIO3_ID, EDIFICIO3_ID);
    TPrimitiva *edificio8 = new TPrimitiva(EDIFICIO3_ID, EDIFICIO3_ID);
    TPrimitiva *edificio9 = new TPrimitiva(EDIFICIO3_ID, EDIFICIO3_ID);

    printf("Creando valla...\n");
    TPrimitiva *valla1 = new TPrimitiva(VALLA_ID, VALLA_ID);
    printf("Creando fuente...\n");
    TPrimitiva *fuente = new TPrimitiva(FUENTE_ID, FUENTE_ID);
    printf("Creando arbusto...\n");
    TPrimitiva *arbusto = new TPrimitiva(ARB_FUENTE_ID, ARB_FUENTE_ID);
    printf("Creando farolas...\n");
    TPrimitiva *farola = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola2 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola3 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola4 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola5 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola6 = new TPrimitiva(FAROLA_ID, FAROLA_ID);

    TPrimitiva *farola7 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola8 = new TPrimitiva(FAROLA_ID, FAROLA_ID);


    printf("Creando bancos...\n");
    TPrimitiva *banco = new TPrimitiva(BANCO_ID, BANCO_ID);
    TPrimitiva *banco2 = new TPrimitiva(BANCO_ID, BANCO_ID);
    TPrimitiva *banco3 = new TPrimitiva(BANCO_ID, BANCO_ID);
    TPrimitiva *banco4 = new TPrimitiva(BANCO_ID, BANCO_ID);
    TPrimitiva *banco5 = new TPrimitiva(BANCO_ID, BANCO_ID);
    TPrimitiva *banco6 = new TPrimitiva(BANCO_ID, BANCO_ID);

    printf("Creando basuras...\n");
    TPrimitiva *basura = new TPrimitiva(BASURA_ID, BASURA_ID);
    TPrimitiva *basura2 = new TPrimitiva(BASURA_ID, BASURA_ID);
    TPrimitiva *basura3 = new TPrimitiva(BASURA_ID, BASURA_ID);
    TPrimitiva *basura4 = new TPrimitiva(BASURA_ID, BASURA_ID);

    printf("Creando campo de futbol...\n");
    TPrimitiva *campoFutbol = new TPrimitiva(CAMPO_ID, CAMPO_ID);
    printf("Campo de futbol creado OK\n");


    car->colores[0][0] = 0.1;
    car->colores[0][1] = 0.4;
    car->colores[0][2] = 0.9;
    car->colores[0][3] = 1.0;

    car->tx = 18;
    car->tz = 18;
    car->ty= 120;

    car2->ty=-120;
    car2->tx =-18;
    car2->tz = 18;

    // Carretera (gris oscuro)
    road->colores[0][0] = 0.1;
    road->colores[0][1] = 0.1;
    road->colores[0][2] = 0.1;
    road->colores[0][3] = 1.0;

    // Líneas (blancas)
    road->colores[1][0] = 1.0;
    road->colores[1][1] = 1.0;
    road->colores[1][2] = 1.0;
    road->colores[1][3] = 1.0;

    // Césped (verde)
    road->colores[2][0] = 0.2;
    road->colores[2][1] = 0.5;
    road->colores[2][2] = 0.2;
    road->colores[2][3] = 1.0;

    // Césped (verde)
    campoFutbol->colores[2][0] = 1.0;
    campoFutbol->colores[2][1] = 1.0;
    campoFutbol->colores[2][2] = 1.0;
    campoFutbol->colores[2][3] = 1.0;



    edificio1->colores[0][0] = 1.0; // R
    edificio1->colores[0][1] = 1.0; // G
    edificio1->colores[0][2] = 0.0; // B
    edificio1->ty=-45;

    edificio4->colores[0][0] = 1.0; // R
    edificio4->colores[0][1] = 1.0; // G
    edificio4->colores[0][2] = 0.0; //

    edificio9->colores[0][0] = 1.0; // R
    edificio9->colores[0][1] = 1.0; // G
    edificio9->colores[0][2] = 0.0; // B

    edificio2->ty=45;
    edificio2->colores[0][2]=0.8;
    edificio5->colores[0][2]=0.8;
    edificio7->colores[0][2]=0.8;

    edificio3->colores[0][0] = 1.0; // R
    edificio3->colores[0][1] = 0.4; // G
    edificio3->colores[0][2] = 0.8; //

    edificio6->colores[0][0] = 1.0; // R
    edificio6->colores[0][1] = 0.4; // G
    edificio6->colores[0][2] = 0.8; // B

    edificio8->colores[0][0] = 1.0; // R
    edificio8->colores[0][1] = 0.4; // G
    edificio8->colores[0][2] = 0.8; // B

    valla1->ty=-8;
    valla1->tx=0;
    valla1->colores[1][0] = 1.0;
    valla1->colores[1][1] = 1.0;
    valla1->colores[1][2] = 1.0;
    valla1->colores[1][3] = 1.0;

    fuente->colores[0][0] = 0.75;
    fuente->colores[0][1] = 0.75;
    fuente->colores[0][2] = 0.75;
    fuente->colores[0][3] = 1.0;
    fuente->ty=-10;


    edificio5->tx=-60;
    edificio6->tx=-120;

    edificio7->rz=180;
    edificio7->tx=425;
    edificio7->ty=-300;

    edificio8->rz=180;
    edificio8->tx=365;
    edificio8->ty=-300;

    edificio9->rz=180;
    edificio9->tx=305;
    edificio9->ty=-300;


    banco-> tx = -10;
    banco2->ty=90;
    banco2->tx = - 10;

    banco3-> tx= 0;
    banco3-> ty= 300;
    banco3-> rz= 180;

    banco4-> tx= 0;
    banco4-> ty= 200;
    banco4-> rz= 180;

    banco6->rz=90;
    banco6->tx = -100;
    banco6-> ty = -30;

    banco5->rz=90;
    banco5->tx = -20;
    banco5-> ty = -30;

    farola2->ty=150;
    farola3->ty=400;
    farola4->ty=330;

    farola7->ty=400;
    farola7->tx=180;
    farola8->tx=180;
    farola8->ty=330;

    farola5->ty=380;
    farola5->tx=-100;
    farola6->ty=380;
    farola6->tx=-130;

    // Césped (verde)
    arbusto->colores[0][0] = 0.2; // R
    arbusto->colores[0][1] = 0.5; // G
    arbusto->colores[0][2] = 0.2; // B
    arbusto->ty=-10;

    basura2->ty=260;
    basura3->ty=180;
    basura3->tx=-120;

    basura4->ty=260;
    basura4->tx=180;

    // A�adir objetos
    AddObject(road);
    AddObject(edificio1);
    AddObject(edificio2);
    AddObject(edificio3);

    AddObject(edificio4);
    AddObject(edificio5);
    AddObject(edificio6);
    AddObject(edificio7);
    AddObject(edificio8);
    AddObject(edificio9);

    AddObject(valla1);
    AddObject(fuente);
    AddObject(arbusto);

    AddObject(banco);
    AddObject(banco2);
    AddObject(banco3);
    AddObject(banco4);
    AddObject(banco5);
    AddObject(banco6);


    AddObject(farola);
    AddObject(farola2);
    AddObject(farola3);
    AddObject(farola4);
    AddObject(farola5);
    AddObject(farola6);

    AddObject(farola7);
    AddObject(farola8);

    AddObject(basura);
    AddObject(basura2);
    AddObject(basura3);
    AddObject(basura4);

    AddObject(campoFutbol);

    // A�adir coches
    AddCar(car);
    AddCar(car2);
}

//************************************************************** Clase TGui

TGui::TGui()
{
    sel = 1;
    sel_camara = 0;
    enable_panel2 = 1;
    light0_enabled = 1;
    light1_enabled = 1;
    light2_enabled = 1;
    light0_intensity = 0.8;
    light1_intensity = 0.8;
    light2_intensity = 0.8;
    specular_intensity = 0.1;  // Intensidad especular inicial
    shininess = 64.0;          // Shininess inicial
    memcpy(light0_position, light0_position_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));
    memcpy(light2_position, light2_position_c, 4*sizeof(float));
}

void controlCallback(int control)
{
    gui.ControlCallback(control);
}

void __fastcall TGui::Init(int main_window) {

    /****************************************************/
    /*        C�digo con el interfaz Gr�fico GLUI       */
    /****************************************************/
    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

    window_id = main_window;

    /*** Crea a ventana lateral ***/
    glui = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_RIGHT );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    // A�ade un panel con texto con el valor de la selecci�n
    GLUI_Panel *panel0 = new GLUI_Panel(glui, "Seleccion");
    GLUI_RadioGroup *radioGroup = new GLUI_RadioGroup(panel0, &sel, SEL_ID, controlCallback);
    glui->add_radiobutton_to_group(radioGroup, "NINGUNO");


    glui->add_radiobutton_to_group(radioGroup, "COCHE 1");
    glui->add_radiobutton_to_group(radioGroup, "COCHE 2");

    //anade una separacion
    new GLUI_StaticText( glui, "" );

    // A�ade un panel con texto con el valor de la selecci�n
    GLUI_Panel *panel1= new GLUI_Panel(glui, "Tipo de vista");
    GLUI_RadioGroup *radioGroup2 = new GLUI_RadioGroup(panel1, &sel_camara, CAMARA_ID, controlCallback);

    glui->add_radiobutton_to_group(radioGroup2, "LIBRE");
    glui->add_radiobutton_to_group(radioGroup2, "AEREA");
    glui->add_radiobutton_to_group(radioGroup2, "TERCERA PERSONA");

    //anade una separacion
    new GLUI_StaticText( glui, "" );

    // A�ade un panel con texto con el valor de la selecci�n
    GLUI_Panel *panel3= new GLUI_Panel(glui, "Proyeccion");
    GLUI_RadioGroup *radioGroup3 = new GLUI_RadioGroup(panel3, &escena.proyeccion, PROYECCION_ID, controlCallback);

    glui->add_radiobutton_to_group(radioGroup3, "PERSPECTIVA");
    glui->add_radiobutton_to_group(radioGroup3, "PARALELA");

    //anade una separacion
    new GLUI_StaticText( glui, "" );

    // Panel para el sentido de las caras
    GLUI_Panel *panel4 = new GLUI_Panel(glui, "Sentido de caras");
    GLUI_RadioGroup *radioGroup4 = new GLUI_RadioGroup(panel4, &escena.winding_order, WINDING_ID, controlCallback);

    glui->add_radiobutton_to_group(radioGroup4, "ANTIHORARIO (CCW)");
    glui->add_radiobutton_to_group(radioGroup4, "HORARIO (CW)");

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    // Panel para el modo de visualizaci�n
    GLUI_Panel *panel_vis = new GLUI_Panel(glui, "Modo de visualizacion");
    GLUI_RadioGroup *radioGroup_vis = new GLUI_RadioGroup(panel_vis, &escena.wireframe, MODO_VIS_ID, controlCallback);

    glui->add_radiobutton_to_group(radioGroup_vis, "SOLIDO");
    glui->add_radiobutton_to_group(radioGroup_vis, "ALAMBRICO");
    glui->add_radiobutton_to_group(radioGroup_vis, "PUNTOS");

    new GLUI_StaticText( glui, "" );

    obj_panel = new GLUI_Rollout(glui, "Sombreado", false );

    /***** Control para el tipo de sombreado *****/
    GLUI_RadioGroup *radioGroup_shader = new GLUI_RadioGroup(obj_panel, &escena.shader, SOMBREADO_ID, controlCallback);
    glui->add_radiobutton_to_group(radioGroup_shader, "Plano (Flat)");
    glui->add_radiobutton_to_group(radioGroup_shader, "Gouraud (Por vertice)");
    glui->add_radiobutton_to_group(radioGroup_shader, "Phong (Por fragmento)");

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    obj_panel = new GLUI_Rollout(glui, "Propiedades", false );

    /***** Control para las propiedades de escena *****/

    new GLUI_Checkbox( obj_panel, "Culling", &escena.culling, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Z Buffer", &escena.z_buffer, 1, controlCallback );

    /******** A�ade controles para las luces ********/

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Luces", false );

    // Control de luz ambiente
    GLUI_Spinner *ambient_spinner = new GLUI_Spinner(roll_lights, "Luz Ambiente:", &escena.ambient_intensity);
    ambient_spinner->set_float_limits(0.0f, 1.0f);
    ambient_spinner->set_speed(0.01f);
    new GLUI_StaticText(roll_lights, "");

    // Controles para brillo especular
    GLUI_Spinner *specular_spinner = new GLUI_Spinner(roll_lights, "Intensidad Especular:", &specular_intensity, SPECULAR_INTENSITY_ID, controlCallback);
    specular_spinner->set_float_limits(0.0f, 2.0f);
    specular_spinner->set_speed(0.01f);
    
    GLUI_Spinner *shininess_spinner = new GLUI_Spinner(roll_lights, "Brillo (Shininess):", &shininess, SHININESS_ID, controlCallback);
    shininess_spinner->set_float_limits(1.0f, 128.0f);
    shininess_spinner->set_speed(1.0f);
    new GLUI_StaticText(roll_lights, "");

    GLUI_Rollout *light0 = new GLUI_Rollout( roll_lights, "Luz 1", false );
    GLUI_Rollout *light1 = new GLUI_Rollout( roll_lights, "Luz 2", false );
    GLUI_Rollout *light2 = new GLUI_Rollout( roll_lights, "Luz 3", false );

    new GLUI_Checkbox( light0, "Encendida", &light0_enabled, LIGHT0_ENABLED_ID, controlCallback );
    light0_spinner = new GLUI_Spinner( light0, "Intensidad:", &light0_intensity,
                            LIGHT0_INTENSITY_ID, controlCallback );
    light0_spinner->set_float_limits( 0.0, 1.0 );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[0],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[1],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[2],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);

    new GLUI_Checkbox( light1, "Encendida", &light1_enabled, LIGHT1_ENABLED_ID, controlCallback );
    light1_spinner = new GLUI_Spinner( light1, "Intensidad:", &light1_intensity,
                            LIGHT1_INTENSITY_ID, controlCallback );
    light1_spinner->set_float_limits( 0.0, 1.0 );
    sb = new GLUI_Scrollbar( light1, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[0],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[1],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[2],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);

    new GLUI_Checkbox( light2, "Encendida", &light2_enabled, LIGHT2_ENABLED_ID, controlCallback );
    light2_spinner = new GLUI_Spinner( light2, "Intensidad:", &light2_intensity,
                            LIGHT2_INTENSITY_ID, controlCallback );
    light2_spinner->set_float_limits( 0.0, 1.0 );
    sb = new GLUI_Scrollbar( light2, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light2_position[0],LIGHT2_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light2, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light2_position[1],LIGHT2_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light2, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light2_position[2],LIGHT2_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);


    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    /***  Rollout de Opciones ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Opciones", false );
    new GLUI_Checkbox( options, "Dibujar Coche", &escena.show_car );
    new GLUI_Checkbox( options, "Dibujar Ruedas", &escena.show_wheels );
    new GLUI_Checkbox( options, "Dibujar Carretera", &escena.show_road );


    /*** Disable/Enable botones ***/
    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );
    new GLUI_Checkbox( glui, "Permitir Movimiento", &enable_panel2 );
    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Resetear Posicion", RESET_ID, controlCallback );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

     // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    new GLUI_StaticText( glui, "  Autor:" );
    new GLUI_StaticText( glui, "  2012-2022 (C) Juan Antonio Puchol  " );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

    // A�ade una separaci�n
    new GLUI_StaticText( glui, "" );

    /****** A 'quit' button *****/
    new GLUI_Button( glui, "Salir", 0,(GLUI_Update_CB)exit );

    // Crea la subventana inferior
    glui2 = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_BOTTOM );

    /**** Link windows to GLUI, and register idle callback ******/
    glui->set_main_gfx_window( window_id );
    glui2->set_main_gfx_window( window_id );

    view_rot = new GLUI_Rotation(glui2, "Rotacion Escena", escena.view_rotate );
    view_rot->set_spin( 1.0 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy = new GLUI_Translation(glui2, "Traslacion Escena XY", GLUI_TRANSLATION_XY, escena.view_position );
    trans_xy->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Traslacion Escena X", GLUI_TRANSLATION_X, escena.view_position );
    trans_x->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y = new GLUI_Translation( glui2, "Traslacion Escena Y", GLUI_TRANSLATION_Y, &escena.view_position[1] );
    trans_y->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z = new GLUI_Translation( glui2, "Traslacion Escena Z", GLUI_TRANSLATION_Z, &escena.scale );
    trans_z->set_speed( .005 );
    

}

/**************************************** TGui::ControlCallback() *******************/

void __fastcall TGui::ControlCallback( int control )
{
    switch (control) {
        case LIGHT0_ENABLED_ID: {
            if ( light0_enabled )
                light0_spinner->enable();
            else
                light0_spinner->disable();
            break;
        }
        case LIGHT1_ENABLED_ID: {
            if ( light1_enabled )
                light1_spinner->enable();
            else
                light1_spinner->disable();
            break;
        }
        case LIGHT2_ENABLED_ID: {
            if ( light2_enabled )
                light2_spinner->enable();
            else
                light2_spinner->disable();
            break;
        }
        case LIGHT0_INTENSITY_ID: {


            escena.light0_diffuse[0] = light0_diffuse_c[0]*this->light0_intensity;
            escena.light0_diffuse[1] = light0_diffuse_c[1] *this->light0_intensity;
            escena.light0_diffuse[2] = light0_diffuse_c[2] *this->light0_intensity;
            escena.light0_diffuse[3] = light0_diffuse_c[3];
            break;
        }
        case LIGHT1_INTENSITY_ID: {

            escena.light1_diffuse[0] = light1_diffuse_c[0] * light1_intensity;
            escena.light1_diffuse[1] = light1_diffuse_c[1] * light1_intensity;
            escena.light1_diffuse[2] = light1_diffuse_c[2] * light1_intensity;
            escena.light1_diffuse[3] = light1_diffuse_c[3];
            break;
        }
        case LIGHT2_INTENSITY_ID: {

            escena.light2_diffuse[0] = light2_diffuse_c[0] * light2_intensity;
            escena.light2_diffuse[1] = light2_diffuse_c[1] * light2_intensity;
            escena.light2_diffuse[2] = light2_diffuse_c[2] * light2_intensity;
            escena.light2_diffuse[3] = light2_diffuse_c[3];
            break;
        }
        case SPECULAR_INTENSITY_ID: {
            // Actualizar la intensidad especular en el shader
            glUniform1f(escena.uSpecularLocation, specular_intensity);
            break;
        }
        case SHININESS_ID: {
            // Actualizar el shininess en el shader
            glUniform1f(escena.uShininessLocation, shininess);
            break;
        }
        case ENABLE_ID: {
            glui2->enable();
            break;
        }
        case DISABLE_ID: {
            glui2->disable();
            break;
        }
        case RESET_ID: {
            memcpy(escena.view_position,view_position_c,3*sizeof(float));
            view_rot->reset();
            escena.scale = 100.0;
            break;
        }
        case SEL_ID: {
            escena.seleccion = sel;
            //GLUI_Master.SetFocus(true);
            glutSetWindow( glui->get_glut_window_id() );
            break;
        }
        case CAMARA_ID: {
            this->cambiarPosCamara();
            break;
        }
        case SOMBREADO_ID: {
            // El sombreado se maneja automáticamente con la variable escena.shader
            // 0 = Flat (plano)
            // 1 = Gouraud (por vértice) 
            // 2 = Phong (por fragmento)
            break;
        }
  } // switch
}

/***************************************** TGui::Idle() ***********/

void __fastcall TGui::Idle( void )
{
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != window_id )
    glutSetWindow(window_id);

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */
    if (enable_panel2)

        glui2->enable();
    else
        glui2->disable();

  glutPostRedisplay();
}

/**************************************** TGui::reshape() *************/

void __fastcall TGui::Reshape( int xx, int yy )
{
    int x, y, ancho, alto;
    GLUI_Master.get_viewport_area( &x, &y, &ancho, &alto );
    glViewport( x, y, ancho, alto );

    // !!!!! ATENCI�N: comprobar que alto no sea 0, sino divisi�n por 0 !!!!!!
    escena.xy_aspect = (float)ancho / (float)alto;
    escena.projectionMatrix = glm::perspective(45.0f, escena.xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(escena.uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(escena.projectionMatrix));

    //std::cout << "xy aspect: " << escena.xy_aspect << std::endl;

    glutPostRedisplay();
}

/***************************************** gui::motion() **********/

void __fastcall TGui::Motion(int x, int y )
{
    glutPostRedisplay();
}

/***************************************** gui::Mouse() **********/

void __fastcall TGui::Mouse(int button, int button_state, int x, int y )
{
    if(button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN) //si se detecta click izquierdo y estado presionado
    {
        escena.Pick3D(x, y);
    }

}

void TGui::cambiarPosCamara(){
    escena.camara = sel_camara;
    TPrimitiva* coche = escena.GetCar(escena.seleccion);
    switch(sel_camara){

        case 0:
            memcpy(escena.view_position,view_position_c, 3*sizeof(float));
            view_rot->reset();
            escena.scale = 100.00;
            escena.viewBaseMatrix = glm::mat4(1.0f);
            break;

        case 1: //vista aerea
            memcpy(escena.view_position,view_position_c, 3*sizeof(float));
            view_rot->reset();
            escena.scale = 100.00;

            if(coche != NULL)
            {
                glm:: mat4 newView = glm::mat4(1.0f);
                newView = glm::translate(newView,glm::vec3(-coche->tx,-coche->ty , coche->tz-150));
                escena.viewBaseMatrix = newView;
            }
            break;
        case 2: //tercera persona
            memcpy(escena.view_position,view_position_c, 3*sizeof(float));
            view_rot->reset();
            escena.scale = 100.00;

            if(coche != NULL)
            {
                float distancia = 80.0f;  // Distancia detrás del coche
                float altura = 30.0f;     

                // Calcular ángulo del coche en radianes
                float angulo = glm::radians(coche->gc);
                
                //la posicion de la camara se calcula en funcion de la posicion y angulo del coche (por detras)
                float cam_x = coche->tx + distancia * sin(angulo);
                float cam_y = coche->ty - distancia * cos(angulo);

                
                glm::vec3 cam_pos = glm::vec3(cam_x, cam_y, altura);
                glm::vec3 target = glm::vec3(coche->tx, coche->ty, 5.0f); // Mirar al coche
                glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f); // Arriba en Z
                
                escena.viewBaseMatrix = glm::lookAt(cam_pos, target, up);
            }
            break;


    }
}

void TEscena::sendLights(){
    // Luz 0
    // Activar/desactivar la luz 0 (1 = encendida, 0 = apagada)
    glUniform1i(uLuz0Location, gui.light0_enabled);

    // Enviar posición e intensidad difusa de la luz 0 al shader
    glUniform4fv(uLuz0PosLocation, 1, this->light0_position);
    glUniform4fv(uDifusseI0Location, 1, this->light0_diffuse);

    // Luz 1
    // Activar/desactivar la luz 1 (1 = encendida, 0 = apagada)
    glUniform1i(uLuz1Location, gui.light1_enabled);

    // Enviar posición e intensidad difusa de la luz 1 al shader
    glUniform4fv(uLuz1PosLocation, 1, this->light1_position);
    glUniform4fv(uDifusseI1Location, 1, this->light1_diffuse);

    // Luz 2
    // Activar/desactivar la luz 2 (1 = encendida, 0 = apagada)
    glUniform1i(uLuz2Location, gui.light2_enabled);

    // Enviar posición e intensidad difusa de la luz 2 al shader
    glUniform4fv(uLuz2PosLocation, 1, this->light2_position);
    glUniform4fv(uDifusseI2Location, 1, this->light2_diffuse);
}

