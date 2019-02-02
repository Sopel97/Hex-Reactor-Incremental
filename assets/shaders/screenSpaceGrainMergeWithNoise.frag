uniform sampler2D texture;
uniform sampler2D noiseTexture;

varying vec2 screenPos;

void main()
{
	vec4 texPixel = texture2D(texture, gl_TexCoord[0].xy);
	vec4 noisePixel = texture2D(noiseTexture, screenPos);
    if(texPixel.a == 0)
    {
        gl_FragColor = (gl_Color * texPixel);
    }
    else
    {
        gl_FragColor = (gl_Color * texPixel) + noisePixel - 0.5;
    }   
	//gl_FragColor = noisePixel;
}