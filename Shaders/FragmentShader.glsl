
//precision mediump float;	// Precisión media, en algunas gráficas no se soporta (depende de la versión de GLSL), en ese caso comentar o quitar esta línea

varying vec4 v_Color;		// in: color del vertex shader

uniform sampler2D u_Texture; 
varying vec2 v_UV;

void main()
{
    gl_FragColor = v_Color;
	//gl_FragColor = texture2D(u_Texture, v_UV); 
}
