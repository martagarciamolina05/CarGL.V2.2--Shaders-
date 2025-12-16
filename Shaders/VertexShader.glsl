
attribute vec4 a_Position;	        // in: Posición de cada vértice
attribute vec3 a_Normal;	        // in: Normal de cada vértice
attribute vec2 a_UV;	            // in: Coordenadas de textura

uniform mat4 u_ProjectionMatrix; 	// in: Matriz Projection
uniform mat4 u_MVMatrix;	        // in: Matriz ModelView
uniform mat4 u_VMatrix;             // in: Matriz View (cámara)
uniform vec4 u_Color;		        // in: Color del objeto
uniform int  u_Luz0;                // in: Indica si la luz 0 está encedida
uniform int  u_Luz1;                // in: Indica si la luz 1 está encedida
uniform int  u_Luz2;                // in: Indica si la luz 2 está encedida
uniform int  u_UseTexture;          // in: Indica si el objeto usa textura
uniform float u_Retiling;           // in: Factor de repetición de la textura

varying vec4 v_Color;		        // out: Color al fragment shader
varying vec2 v_UV;                  // out: Coordenadas UV al fragment shader

uniform vec4 u_Light0Pos;
uniform vec4 u_DiffuseI0;
uniform vec4 u_Light1Pos;
uniform vec4 u_DiffuseI1;
uniform vec4 u_Light2Pos;
uniform vec4 u_DiffuseI2;

uniform float u_Ambient;  // Intensidad de luz ambiente

void main()
{
        vec3 P = vec3(u_MVMatrix * a_Position);	            // Posición del vértice
	vec3 N = vec3(u_MVMatrix * vec4(a_Normal, 0.0));    // Normal del vértice

	vec4 diffuseTerm = vec4(0.0);

	// Luz 0
	if (u_Luz0>0) {                                     // Si la luz 0 está encendida se calcula la intesidad difusa
        vec4 LightPos0 = u_VMatrix*u_Light0Pos;	        // Posición de la luz 0 [fija]
        float d0 = length(LightPos0.xyz - P);			// distancia de la luz 0
	    vec3  L0 = normalize(LightPos0.xyz - P);		// Vector Luz 0

        float NdotL0 = max(dot(L0,N), 0.0);

        // Cálculo de la atenuación
        float attenuation0 = 80.0/(0.25+(0.01*d0)+(0.003*d0*d0));
        diffuseTerm += u_DiffuseI0 * NdotL0 * attenuation0;
	}

	// Luz 1
	if (u_Luz1>0) {                                     // Si la luz 1 está encendida se calcula la intesidad difusa
        vec4 LightPos1 = u_VMatrix*u_Light1Pos;	        // Posición de la luz 1 [fija]
        float d1 = length(LightPos1.xyz - P);			// distancia de la luz 1
	    vec3  L1 = normalize(LightPos1.xyz - P);		// Vector Luz 1

        float NdotL1 = max(dot(L1,N), 0.0);

        // Cálculo de la atenuación
        float attenuation1 = 80.0/(0.25+(0.01*d1)+(0.003*d1*d1));
        diffuseTerm += u_DiffuseI1 * NdotL1 * attenuation1;
	}

	// Luz 2
	if (u_Luz2>0) {                                     // Si la luz 2 está encendida se calcula la intesidad difusa
        vec4 LightPos2 = u_VMatrix*u_Light2Pos;	        // Posición de la luz 2 [fija]
        float d2 = length(LightPos2.xyz - P);			// distancia de la luz 2
	    vec3  L2 = normalize(LightPos2.xyz - P);		// Vector Luz 2

        float NdotL2 = max(dot(L2,N), 0.0);

        // Cálculo de la atenuación
        float attenuation2 = 80.0/(0.25+(0.01*d2)+(0.003*d2*d2));
        diffuseTerm += u_DiffuseI2 * NdotL2 * attenuation2;
	}

	v_Color = (u_Color * u_Ambient) + (u_Color * diffuseTerm);
	v_Color.a = u_Color.a;
	
	gl_Position = u_ProjectionMatrix * vec4(P, 1.0);

        v_UV = a_UV * u_Retiling;
        //gl_Position = u_ProjectionMatrix * vec4(a_Position.xyz, 1.0);
}
