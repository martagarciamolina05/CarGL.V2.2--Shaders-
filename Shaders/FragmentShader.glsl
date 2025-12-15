
//precision mediump float;	// Precisión media, en algunas gráficas no se soporta (depende de la versión de GLSL), en ese caso comentar o quitar esta línea

varying vec4 v_Color;		// in: color del vertex shader

uniform sampler2D u_Texture; 
varying vec2 v_UV;

void main()
{
	// Descomentar la siguiente línea para ver las coordenadas UV como colores (debug)
	//gl_FragColor = vec4(v_UV, 0.0, 1.0);
	
	gl_FragColor = texture2D(u_Texture, v_UV) * v_Color;
}