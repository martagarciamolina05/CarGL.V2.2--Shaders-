
//precision mediump float;	// Precisión media, en algunas gráficas no se soporta (depende de la versión de GLSL), en ese caso comentar o quitar esta línea

varying vec4 v_Color;		// in: color del vertex shader
varying vec2 v_UV;			// in: coordenadas de textura
varying vec3 v_Position;	// in: posición del vértice en view space
varying vec3 v_Normal;		// in: normal del vértice en view space

uniform sampler2D u_Texture;
uniform int u_UseTexture;	// indica si se debe usar textura

// Uniforms para el cálculo especular
uniform vec4 u_Light0Pos;
uniform vec4 u_DiffuseI0;
uniform vec4 u_Light1Pos;
uniform vec4 u_DiffuseI1;
uniform vec4 u_Light2Pos;
uniform vec4 u_DiffuseI2;

uniform int  u_Luz0;
uniform int  u_Luz1;
uniform int  u_Luz2;

uniform mat4 u_VMatrix;             // Matriz View para transformar luces

uniform float u_SpecularIntensity;  // Intensidad del brillo especular (ks)
uniform float u_Shininess;          // Exponente de shininess (concentración del brillo)

void main()
{
	// Si el objeto tiene textura, aplicarla; si no, usar solo el color
	vec4 finalColor;
	if (u_UseTexture != 0) {
		vec4 texColor = texture2D(u_Texture, v_UV);
		finalColor = texColor * v_Color;
	} else {
		finalColor = v_Color;
	}
	
	// Cálculo del término especular
	vec3 N = normalize(v_Normal);
	vec3 V = normalize(-v_Position); // Vector hacia la cámara (en view space la cámara está en el origen)
	
	float specularTerm = 0.0;
	
	// Luz 0 - Componente especular
	if (u_Luz0 > 0) {
		vec4 LightPos0 = u_VMatrix * u_Light0Pos;
		float d0 = length(LightPos0.xyz - v_Position);
		vec3 L0 = normalize(LightPos0.xyz - v_Position);
		vec3 R0 = reflect(-L0, N); // Vector de reflexión
		
		float specular0 = pow(max(dot(R0, V), 0.0), u_Shininess);
		float attenuation0 = 80.0/(0.25+(0.01*d0)+(0.003*d0*d0));
		specularTerm += specular0 * attenuation0;
	}
	
	// Luz 1 - Componente especular
	if (u_Luz1 > 0) {
		vec4 LightPos1 = u_VMatrix * u_Light1Pos;
		float d1 = length(LightPos1.xyz - v_Position);
		vec3 L1 = normalize(LightPos1.xyz - v_Position);
		vec3 R1 = reflect(-L1, N);
		
		float specular1 = pow(max(dot(R1, V), 0.0), u_Shininess);
		float attenuation1 = 80.0/(0.25+(0.01*d1)+(0.003*d1*d1));
		specularTerm += specular1 * attenuation1;
	}
	
	// Luz 2 - Componente especular
	if (u_Luz2 > 0) {
		vec4 LightPos2 = u_VMatrix * u_Light2Pos;
		float d2 = length(LightPos2.xyz - v_Position);
		vec3 L2 = normalize(LightPos2.xyz - v_Position);
		vec3 R2 = reflect(-L2, N);
		
		float specular2 = pow(max(dot(R2, V), 0.0), u_Shininess);
		float attenuation2 = 80.0/(0.25+(0.01*d2)+(0.003*d2*d2));
		specularTerm += specular2 * attenuation2;
	}
	
	// Aplicar el término especular (blanco puro) multiplicado por la intensidad
	// El brillo especular es típicamente blanco, sin color de la luz
	vec3 specularColor = vec3(1.0) * specularTerm * u_SpecularIntensity;
	
	// Limitar el brillo especular para evitar saturación
	specularColor = clamp(specularColor, 0.0, 1.0);
	
	// Combinar color difuso + textura con el brillo especular
	gl_FragColor = vec4(finalColor.rgb + specularColor, finalColor.a);
}