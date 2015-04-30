#ifdef GL_FRAGMENT_PRECISION_HIGH
    // Default precision
    precision highp float;
#else
    precision mediump float;
#endif

// In ES 2.0 gl_FragColor it's equivalent to "out vec4 color;" from the tutorial
void main()
{
    gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}