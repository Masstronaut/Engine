import { Component, ViewChild, Inject, ElementRef } from '@angular/core';
import { GoldenLayoutComponentState, GlOnResize, GlOnHide, GlOnShow, GoldenLayoutContainer } from 'ng-golden-layout';
import * as GoldenLayout from 'golden-layout';
import { mat4, glMatrix } from 'gl-matrix-ts';

@Component({
  selector: 'scene',
  template: `
    <canvas #editorView></canvas>
  `
})
export class SceneViewerComponent implements GlOnResize {

  @ViewChild('editorView') editorView: ElementRef

  constructor(@Inject(GoldenLayoutComponentState) private state: any,
              @Inject(GoldenLayoutContainer) private container: GoldenLayout.Container) {}

  canvas : HTMLCanvasElement;
  gl: WebGL2RenderingContext;
  world: any;
  program: WebGLProgram;

  ngAfterViewInit() {
    this.canvas = (<HTMLCanvasElement>this.editorView.nativeElement);
    // WebGL context
   this.gl = this.canvas.getContext("webgl2");
    if(!this.gl) {
      console.error("WebGL2 is not supported");
    }
    
    let vertText = [
    'attribute vec3 pos;',
    'attribute vec3 color;',
    'varying vec3 fragColor;',
    'uniform mat4 world;',
    'uniform mat4 view;',
    'uniform mat4 proj;',
    'void main()',
    '{',
    '   fragColor = color;',
    '   gl_Position = proj * view * world * vec4(pos, 1.0);',
    '}'
    ].join('\n');
    
    let fragText = [
    'precision mediump float;',
    'varying vec3 fragColor;',
    'void main()',
    '{',
    '   gl_FragColor = vec4(fragColor, 1.0);',
    '}'
    ].join('\n');


    this.gl.clearColor(.75, 0.85, 0.8, 1.0); 
    this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);
    
    this.gl.enable(this.gl.DEPTH_TEST);
    this.gl.enable(this.gl.CULL_FACE);

    // Create Shaders
    let vertShader = this.gl.createShader(this.gl.VERTEX_SHADER);
    let fragShader = this.gl.createShader(this.gl.FRAGMENT_SHADER);
    
    this.gl.shaderSource(vertShader, vertText);
    this.gl.shaderSource(fragShader, fragText);

    this.gl.compileShader(vertShader);
    
    if(!this.gl.getShaderParameter(vertShader, this.gl.COMPILE_STATUS)){
        console.error('ERROR COMPILING VERT SHADER', this.gl.getShaderInfoLog(vertShader));
    }

    this.gl.compileShader(fragShader);
    
    if(!this.gl.getShaderParameter(fragShader, this.gl.COMPILE_STATUS)){
        console.error('ERROR COMPILING FRAG SHADER', this.gl.getShaderInfoLog(fragShader));
    }

    this.program = this.gl.createProgram();
    this.gl.attachShader(this.program, vertShader);
    this.gl.attachShader(this.program, fragShader);
    this.gl.linkProgram(this.program);
    if(!this.gl.getProgramParameter(this.program, this.gl.LINK_STATUS)) {
        console.error('ERROR LINKING PROGRAM', this.gl.getProgramInfoLog(this.program));
    }

    this.gl.validateProgram(this.program);
    if(!this.gl.getProgramParameter(this.program, this.gl.VALIDATE_STATUS)) {
        console.error('ERROR VALIDATING PROGRAM', this.gl.getProgramInfoLog(this.program));
    }
    
    //Create Verts
    let cubeVerts = 
	[ // X, Y, Z           R, G, B
		// Top
		-1.0, 1.0, -1.0,   0.5, 0.5, 0.5,
		-1.0, 1.0, 1.0,    0.5, 0.5, 0.5,
		1.0, 1.0, 1.0,     0.5, 0.5, 0.5,
		1.0, 1.0, -1.0,    0.5, 0.5, 0.5,

		// Left
		-1.0, 1.0, 1.0,    0.75, 0.25, 0.5,
		-1.0, -1.0, 1.0,   0.75, 0.25, 0.5,
		-1.0, -1.0, -1.0,  0.75, 0.25, 0.5,
		-1.0, 1.0, -1.0,   0.75, 0.25, 0.5,

		// Right
		1.0, 1.0, 1.0,    0.25, 0.25, 0.75,
		1.0, -1.0, 1.0,   0.25, 0.25, 0.75,
		1.0, -1.0, -1.0,  0.25, 0.25, 0.75,
		1.0, 1.0, -1.0,   0.25, 0.25, 0.75,

		// Front
		1.0, 1.0, 1.0,    1.0, 0.0, 0.15,
		1.0, -1.0, 1.0,    1.0, 0.0, 0.15,
		-1.0, -1.0, 1.0,    1.0, 0.0, 0.15,
		-1.0, 1.0, 1.0,    1.0, 0.0, 0.15,

		// Back
		1.0, 1.0, -1.0,    0.0, 1.0, 0.15,
		1.0, -1.0, -1.0,    0.0, 1.0, 0.15,
		-1.0, -1.0, -1.0,    0.0, 1.0, 0.15,
		-1.0, 1.0, -1.0,    0.0, 1.0, 0.15,

		// Bottom
		-1.0, -1.0, -1.0,   0.5, 0.5, 1.0,
		-1.0, -1.0, 1.0,    0.5, 0.5, 1.0,
		1.0, -1.0, 1.0,     0.5, 0.5, 1.0,
		1.0, -1.0, -1.0,    0.5, 0.5, 1.0,
    ];
    
    let cubeIndices =
	[
		// Top
		0, 1, 2,
		0, 2, 3,

		// Left
		5, 4, 6,
		6, 4, 7,

		// Right
		8, 9, 10,
		8, 10, 11,

		// Front
		13, 12, 14,
		15, 14, 12,

		// Back
		16, 17, 18,
		16, 18, 19,

		// Bottom
		21, 20, 22,
		22, 20, 23
	];
    //Create vbo
    let cubeVBO = this.gl.createBuffer();
    this.gl.bindBuffer(this.gl.ARRAY_BUFFER, cubeVBO);
    this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array(cubeVerts), this.gl.STATIC_DRAW);
    //Create ebo
    let cubeEBO = this.gl.createBuffer();
    this.gl.bindBuffer(this.gl.ELEMENT_ARRAY_BUFFER, cubeEBO);
    this.gl.bufferData(this.gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(cubeIndices), this.gl.STATIC_DRAW);

    let positionAttrLoc = this.gl.getAttribLocation(this.program, 'pos');
    let colorAttrLoc = this.gl.getAttribLocation(this.program, 'color');
    
    this.gl.vertexAttribPointer(positionAttrLoc, 3, this.gl.FLOAT, false, 6 * Float32Array.BYTES_PER_ELEMENT, 0);
    this.gl.vertexAttribPointer(colorAttrLoc, 3, this.gl.FLOAT, false, 6 * Float32Array.BYTES_PER_ELEMENT, 3 * Float32Array.BYTES_PER_ELEMENT);
    
    this.gl.enableVertexAttribArray(positionAttrLoc);
    this.gl.enableVertexAttribArray(colorAttrLoc);

    this.gl.useProgram(this.program);

    let worldUniLoc = this.gl.getUniformLocation(this.program, 'world');
    let viewUniLoc = this.gl.getUniformLocation(this.program, 'view');
    let projUniLoc = this.gl.getUniformLocation(this.program, 'proj');

    this.world = mat4.create();
    let viewMat = mat4.create();
    let projMat = mat4.create();
    
    mat4.identity(this.world);
    mat4.lookAt(viewMat, [0,0,-8], [0,0,0], [0,1,0]);
    mat4.perspective(projMat, glMatrix.toRadian(45), 1920/1080, 0.1, 1000.0);

    this.gl.uniformMatrix4fv(worldUniLoc, false, this.world);
    this.gl.uniformMatrix4fv(viewUniLoc, false, viewMat);
    this.gl.uniformMatrix4fv(projUniLoc, false, projMat);

    this.update();

  }
  

  update() {
    window.requestAnimationFrame(() => this.update());
    let worldUniLoc = this.gl.getUniformLocation(this.program, 'world');
    let rotX = mat4.create();
    let rotY = mat4.create();
    let identity = mat4.create();
    mat4.identity(identity);
    let angle = performance.now() / 1000 / 6 * 2 * Math.PI;
    mat4.rotate(rotY, identity, angle, [0,1,0]);
    mat4.rotate(rotX, identity, angle/4, [1,0,0]);
    mat4.mul(this.world, rotY, rotX);
    this.gl.uniformMatrix4fv(worldUniLoc, false, this.world);  

    this.gl.clearColor(.75, 0.85, 0.8, 1.0); 
    this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);

    this.gl.drawElements(this.gl.TRIANGLES, 36, this.gl.UNSIGNED_SHORT, 0);
  }

  public onInput(e: Event): void {
    
    this.container.extendState({
      value: (<HTMLInputElement>e.target).value
    });
    
    console.log('state saved.');
  }
  
  public glOnResize(): void {
    this.canvas.width = this.container.width;
    this.canvas.height = this.container.height;
    this.gl.viewport(0,0, this.container.width, this.container.height);
    //this.gl.clearColor(.75, 0.85, 0.8, 1.0); 
    //this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);
  }

}