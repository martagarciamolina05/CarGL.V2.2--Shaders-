
//precision mediump float;	// Precisión media, en algunas gráficas no se soporta (depende de la versión de GLSL), en ese caso comentar o quitar esta línea

varying vec4 v_Color;		// in: color del vertex shader
varying vec2 v_UV;			// in: coordenadas de textura

uniform sampler2D u_Texture;
uniform int u_UseTexture;	// indica si se debe usar textura

void main()
{
	// Si el objeto tiene textura, aplicarla; si no, usar solo el color
	if (u_UseTexture != 0) {
		vec4 texColor = texture2D(u_Texture, v_UV);
		gl_FragColor = texColor * v_Color;
	} else {
		gl_FragColor = v_Color;
	}
}