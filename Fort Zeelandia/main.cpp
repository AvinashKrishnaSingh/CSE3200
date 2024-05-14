#define STB_IMAGE_IMPLEMENTATION
#include <GL/glut.h>
#include "stb_image.h"
#include <cmath>
#include <iostream>

GLuint grassTexture; 		// Variable to store the grass texture ID
GLuint pavementTexture; 	// Variable to store the pavement texture ID
GLuint skyTexture; 			// Variable to store the sky texture ID
GLuint fortTexture; 		// Variable to store the fort texture ID
GLuint stairTexture; 		// Variable to store the fort texture ID


/*--------------------------------------------------------------------------------------------------------------------------------------*/



// Global variables
char title[] = "Fort Zeelandia";
GLfloat angle = 0.0f;  										// Rotation angle for the cube
GLfloat cameraX = 0.0f, cameraY = 3.0f, cameraZ = 10.0f; 	// Initial camera position
GLfloat cameraAngleY = 0.0f;								 // Initial camera angle around Y-axis


/*--------------------------------------------------------------------------------------------------------------------------------------*/

/* Function to load texture using stb_image */
void loadTexture(const char *filename, GLuint &texture) {
    int width, height, channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);
    if (!image) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return;
    }

    glGenTextures(1, &texture); // Generate texture ID
    glBindTexture(GL_TEXTURE_2D, texture); // Bind the texture
    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // Free image memory
    stbi_image_free(image);
}

/*-----------------------------------------------------------Funtion  for cannon stair---------------------------------------------------------------------------*/

void drawStaircase(float x, float y, float z, float width, float height, float depth, int numSteps) {
    float stepHeight = height / numSteps;
    float stepDepth = depth / numSteps;
    glBindTexture(GL_TEXTURE_2D, stairTexture);

    
    // Loop to draw each step
    for (int i = 0; i < numSteps; ++i) {
        float stepY = y + i * stepHeight;
        float stepZ = z - i * stepDepth;
        
        glBegin(GL_QUADS);
        // Front face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - width/2, stepY, stepZ + stepDepth);   // Bottom left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width/2, stepY, stepZ + stepDepth);   // Bottom right
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width/2, stepY + stepHeight, stepZ + stepDepth);   // Top right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x - width/2, stepY + stepHeight, stepZ + stepDepth);   // Top left
        
        // Back face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - width/2, stepY, stepZ);   // Bottom left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width/2, stepY, stepZ);   // Bottom right
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width/2, stepY + stepHeight, stepZ);   // Top right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x - width/2, stepY + stepHeight, stepZ);   // Top left
        
        // Right side face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width/2, stepY, stepZ + stepDepth);   // Bottom right of front face
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width/2, stepY, stepZ);   // Bottom right of back face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width/2, stepY + stepHeight, stepZ);   // Top right of back face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width/2, stepY + stepHeight, stepZ + stepDepth);   // Top right of front face
        
        // Left side face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - width/2, stepY, stepZ + stepDepth);   // Bottom left of front face
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - width/2, stepY, stepZ);   // Bottom left of back face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x - width/2, stepY + stepHeight, stepZ);   // Top left of back face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x - width/2, stepY + stepHeight, stepZ + stepDepth);   // Top left of front face
        
        // Top face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - width/2, stepY + stepHeight, stepZ + stepDepth);   // Front left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width/2, stepY + stepHeight, stepZ + stepDepth);   // Front right
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width/2, stepY + stepHeight, stepZ);   // Back right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x - width/2, stepY + stepHeight, stepZ);   // Back left
        
        // Bottom face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - width/2, stepY, stepZ + stepDepth);   // Front left
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width/2, stepY, stepZ + stepDepth);   // Front right
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width/2, stepY, stepZ);   // Back right
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x - width/2, stepY, stepZ);   // Back left
        glEnd();
    }
}

/*-----------------------------------------------------------function for cannon---------------------------------------------------------------------------*/


void drawCylinder(float radius, float height, int slices, int stacks) {
    GLUquadric *quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluCylinder(quad, radius, radius, height, slices, stacks);
    gluDeleteQuadric(quad);
}

void drawCannon(float posX, float posY, float posZ) {
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f); // Set color to black for cannon
    glTranslatef(posX, posY, posZ); // Adjust position
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.1, 1.0, 20, 20); // Cannon barrel
    glTranslatef(0.0f, 0.0f, 1.0f); // Adjust position for cannonball
    glutSolidSphere(0.2, 20, 20); // Cannonball
    glPopMatrix();

    // Reset color to white
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for subsequent objects
}


/*--------------------------------------------------------------------------------------------------------------------------------------*/



/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // Set background color to sky blue
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    // Load grass texture
    loadTexture("Texture/grass_texture.jpg", grassTexture);
    // Load pavement texture
    loadTexture("Texture/pavement_texture.jpg", pavementTexture);
    // Load sky texture
    loadTexture("Texture/sky_texture.jpg", skyTexture);
    //Load fort texture
    loadTexture("Texture/fort_texture.jpg", fortTexture);


}

/*--------------------------------------------------------------------------------------------------------------------------------------*/


/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					// Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     									// To operate on model-view matrix
    glLoadIdentity();               									// Reset the modelview matrix
    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Set up the camera position and orientation

/*--------------------------------------------------Handler for Sky------------------------------------------------------------------------------------*/
    // Render the skybox
    glEnable(GL_TEXTURE_2D); 					// Enable texture mapping
    glBindTexture(GL_TEXTURE_2D, skyTexture); // Bind sky texture
    glBegin(GL_QUADS);
    // Front face of skybox
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 10.0f, -10.0f);  // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 10.0f, -10.0f); // Top left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);  // Bottom left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);   // Bottom right
    // Right face of skybox
    glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 10.0f, 10.0f);   // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 10.0f, -10.0f);  // Top left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);   // Bottom left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 0.0f, 10.0f);    // Bottom right
    // Back face of skybox
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 10.0f, 10.0f);   // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 10.0f, 10.0f);    // Top left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, 10.0f);     // Bottom left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);    // Bottom right
    // Left face of skybox
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 10.0f, -10.0f);  // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 10.0f, 10.0f);   // Top left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);    // Bottom left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);   // Bottom right
    // Top face of skybox
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 10.0f, -10.0f);  // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 10.0f, -10.0f);   // Top left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, 10.0f);    // Bottom left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 10.0f, 10.0f);   // Bottom right
    // Bottom face of skybox
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 0.0f, -10.0f);   // Top right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);    // Top left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, 10.0f);     // Bottom left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);    // Bottom right
    glEnd();

/*----------------------------------------------------------Handler for grass----------------------------------------------------------------------------*/

    // Render the grass (textured base)
    glBindTexture(GL_TEXTURE_2D, grassTexture); // Bind grass texture
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, 10.0f);   // Bottom right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);  // Bottom left
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f, 0.0f, -10.0f); // Top left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);  // Top right
    glEnd();

/*----------------------------------------------------------Handler for pavement----------------------------------------------------------------------------*/

    // Render the pavement
    glBindTexture(GL_TEXTURE_2D, pavementTexture); // Bind pavement texture
    glBegin(GL_QUADS);
    // Front face of pavement
    glTexCoord2f(0.0f, 0.0f); glVertex3f(4.0f, 0.0f, 3.5f);    // Bottom right
    glTexCoord2f(5.0f, 0.0f); glVertex3f(-3.5f, 0.0f, 3.5f);   // Bottom left
    glTexCoord2f(5.0f, 5.0f); glVertex3f(-3.5f, 0.1f, -3.5f);  // Top left
    glTexCoord2f(0.0f, 5.0f); glVertex3f(4.0f, 0.1f, -3.5f);   // Top right
    glEnd();
    
/*-----------------------------------------------------------Handler for fort---------------------------------------------------------------------------*/
    
	// Render the fort at a fixed position and orientation
	glPushMatrix();  // Save the current modelview matrix
	glTranslatef(0.0f, 0.0f, 0.0f); // Move the fort along the z-axis to the center of the pavement
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f); // Adjust the rotation angles as needed
	
	// Render the fort geometry
	glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for texture rendering
	glEnable(GL_TEXTURE_2D); // Enable texture mapping
	glBindTexture(GL_TEXTURE_2D, fortTexture); // Bind fort texture
	
	// Define texture tiling factor
	const float tileFactor = 5.0f;
	
	// Draw the fort with texture
	// Front wall
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 2.0f, 0.0f, 2.0f);
	glTexCoord2f(tileFactor, 0.0f); glVertex3f(-2.0f, 0.0f, 2.0f);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(-2.0f, 3.0f, 2.0f);
	glTexCoord2f(0.0f, tileFactor); glVertex3f( 2.0f, 3.0f, 2.0f);
	glEnd();
	
	// Back wall
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 2.0f, 0.0f,-2.0f);
	glTexCoord2f(tileFactor, 0.0f); glVertex3f(-2.0f, 0.0f,-2.0f);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(-2.0f, 3.0f,-2.0f);
	glTexCoord2f(0.0f, tileFactor); glVertex3f( 2.0f, 3.0f,-2.0f);
	glEnd();
	
	// Left wall
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 0.0f, 2.0f);
	glTexCoord2f(tileFactor, 0.0f); glVertex3f(-2.0f, 0.0f,-2.0f);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(-2.0f, 3.0f,-2.0f);
	glTexCoord2f(0.0f, tileFactor); glVertex3f(-2.0f, 3.0f, 2.0f);
	glEnd();
	
	
	// Right wall
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 2.0f, 0.0f, 2.0f);
	glTexCoord2f(tileFactor, 0.0f); glVertex3f( 2.0f, 0.0f,-2.0f);
	glTexCoord2f(tileFactor, tileFactor); glVertex3f( 2.0f, 3.0f,-2.0f);
	glTexCoord2f(0.0f, tileFactor); glVertex3f( 2.0f, 3.0f, 2.0f);
	glEnd();
	
	
	// Draw solid arc at the edge of the right wall
	glPushMatrix();
	glTranslatef(2.0f, 0.0f, 2.0f); // Translate to the center point of the arc
	glRotatef(90.0f, -120.0f, 0.0f, 0.0f); // Rotate the arc so its curved side faces left
	glEnable(GL_TEXTURE_2D); // Enable texture mapping
	glBindTexture(GL_TEXTURE_2D, fortTexture); // Bind fort texture
	glBegin(GL_TRIANGLE_FAN);
	float radius = 1.5f; // Adjust the radius to change the size of the arc
	float angle;
	int numSegments = 90; // Number of segments in the arc
	glTexCoord2f(0.5f, 0.5f); // Center of texture
	glVertex3f(0.0f, 0.0f, 0.0f); // Center point of the arc
	for (int i = 0; i <= numSegments; ++i) {
	    angle = i * (3.14159265358979323846 / 180.0f);
	    glTexCoord2f(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle)); // Texture coordinates
	    glVertex3f(radius * sin(angle), 0.0f, radius * cos(angle)); // Adjust the vertex coordinates
	}
	glEnd();
	glDisable(GL_TEXTURE_2D); // Disable texture mapping
	glPopMatrix();
	
	// Draw solid arc at the back-right corner of the fort
	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -2.0f); // Translate to the center point of the arc on the back-right wall
	glRotatef(90.0f, -120.0f, 0.0f, 0.0f); // Rotate the arc so its curved side faces left
	glEnable(GL_TEXTURE_2D); // Enable texture mapping
	glBindTexture(GL_TEXTURE_2D, fortTexture); // Bind fort texture
	glBegin(GL_TRIANGLE_FAN);
	float radiusb = 1.5f; // Adjust the radius to change the size of the arc
	float angleb;
	int numSegmentsb = 90; // Number of segments in the arc
	glTexCoord2f(0.5f, 0.5f); // Center of texture
	glVertex3f(0.0f, 0.0f, 0.0f); // Center point of the arc
	for (int i = 0; i <= numSegmentsb; ++i) {
	    angleb = i * (3.14159265358979323846 / 180.0f);
	    glTexCoord2f(0.5f + 0.5f * cos(angleb), 0.5f + 0.5f * sin(angleb)); // Texture coordinates
	    glVertex3f(radiusb * sin(angleb), 0.0f, radiusb * cos(angleb)); // Adjust the vertex coordinates
	}
	glEnd();
	glDisable(GL_TEXTURE_2D); // Disable texture mapping
	glPopMatrix();

	// Draw the connecting rectangle
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fortTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(3.3f, 0.0f, 2.0f);    // Bottom right of the rectangle
	glTexCoord2f(tileFactor, 0.0f); glVertex3f(3.3f, 0.0f, -2.0f);   // Bottom left of the rectangle
	glTexCoord2f(tileFactor, tileFactor); glVertex3f(3.3f, 1.0f, -2.0f);  // Top left of the rectangle
	glTexCoord2f(0.0f, tileFactor); glVertex3f(3.3f, 1.0f, 2.0f);   // Top right of the rectangle
	glEnd();
	glDisable(GL_TEXTURE_2D);


		
	glPopMatrix(); // Restore the previous modelview matrix
	
/*-----------------------------------------------------------Handler for cannon---------------------------------------------------------------------------*/

	//Cannon 1
	glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(4.0f, 0.0f, -23.0f); 
    drawStaircase(0.0f, 0.0f, 16.0f, 1.5f, 1.0f, 2.0f, 3); 
    glPopMatrix();
    drawCannon(-4.0f, 1.1f, 8.5f);
    
    // Cannon 2
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-4.0f, 0.0f, -23.0f); // Position relative to the existing cannon
    drawStaircase(0.0f, 0.0f, 16.0f, 1.5f, 1.0f, 2.0f, 3);
    glPopMatrix();
    drawCannon(4.0f, 1.1f, 8.5f);

    // Cannon 3
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -23.0f); // Position relative to the existing cannon
    drawStaircase(0.0f, 0.0f, 16.0f, 1.5f, 1.0f, 2.0f, 3);
    glPopMatrix();
    drawCannon(0.0f, 1.1f, 8.5f);

    // Cannon 4
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-8.0f, 0.0f, -23.0f); // Position relative to the existing cannon
    drawStaircase(0.0f, 0.0f, 16.0f, 1.5f, 1.0f, 2.0f, 3);
    glPopMatrix();
    drawCannon(8.0f, 1.1f, 8.5f);
    
    
/*-----------------------------------------------------Handler for fence---------------------------------------------------------------------------------*/
	// Render the fence
	glPushMatrix();
	float fenceWidth = 9.0f; // Width of the fence
	float fenceHeight = 0.5f; // Height of the fence
	float wallThickness = 0.5f; // Thickness of the fence walls
	float fencePositionX = 0.0f; // X position of the fence relative to the fort
	float fencePositionY = 0.0f; // Y position of the fence relative to the fort
	float fencePositionZ = -0.6f; // Z position of the fence relative to the fort
	float textureTileFactor = 5.0f; // Texture tiling factor
	
	glTranslatef(fencePositionX, fencePositionY, fencePositionZ); // Position the fence relative to the fort
	glEnable(GL_TEXTURE_2D); // Enable texture mapping
	glBindTexture(GL_TEXTURE_2D, pavementTexture); // Bind fort texture
	glColor3f(1.0f, 1.0f, 1.0f); // Set fence color
	glBegin(GL_QUADS);
	// Left fence
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-fenceWidth / 2, 0.0f, fenceWidth / 2);  // Bottom left
	glTexCoord2f(textureTileFactor, 0.0f); glVertex3f(-fenceWidth / 2, 0.0f, -fenceWidth / 2); // Bottom right
	glTexCoord2f(textureTileFactor, textureTileFactor); glVertex3f(-fenceWidth / 2, fenceHeight, -fenceWidth / 2); // Top right
	glTexCoord2f(0.0f, textureTileFactor); glVertex3f(-fenceWidth / 2, fenceHeight, fenceWidth / 2);  // Top left
	// Right fence
	glTexCoord2f(0.0f, 0.0f); glVertex3f(fenceWidth / 2, 0.0f, fenceWidth / 2);   // Bottom right
	glTexCoord2f(textureTileFactor, 0.0f); glVertex3f(fenceWidth / 2, 0.0f, -fenceWidth / 2);  // Bottom left
	glTexCoord2f(textureTileFactor, textureTileFactor); glVertex3f(fenceWidth / 2, fenceHeight, -fenceWidth / 2);  // Top left
	glTexCoord2f(0.0f, textureTileFactor); glVertex3f(fenceWidth / 2, fenceHeight, fenceWidth / 2);   // Top right
	// Back fence
	glTexCoord2f(0.0f, 0.0f); glVertex3f(fenceWidth / 2, 0.0f, -fenceWidth / 2);  // Bottom right
	glTexCoord2f(textureTileFactor, 0.0f); glVertex3f(-fenceWidth / 2, 0.0f, -fenceWidth / 2); // Bottom left
	glTexCoord2f(textureTileFactor, textureTileFactor); glVertex3f(-fenceWidth / 2, fenceHeight, -fenceWidth / 2); // Top left
	glTexCoord2f(0.0f, textureTileFactor); glVertex3f(fenceWidth / 2, fenceHeight, -fenceWidth / 2);  // Top right
	glEnd();
	glDisable(GL_TEXTURE_2D); // Disable texture mapping
	glPopMatrix();
	
	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
	    

}




/*--------------------------------------------------------------------------------------------------------------------------------------*/


/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


/*------------------------------------------------------Handler for camera movement--------------------------------------------------------------------------------*/

/* Function to handle arrow key presses and zooming */
void specialKeys(int key, int x, int y) {
    const float angleStep = 5.0f; // Angle increment for camera rotation
    const float moveStep = 0.1f; // Step for camera movement
    const float zoomStep = 0.5f; // Step for zooming

    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngleY -= angleStep; // Rotate the camera left
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleY += angleStep; // Rotate the camera right
            break;
        case GLUT_KEY_UP:
            // Move the camera up along the y-axis
            cameraY += moveStep;
            break;
        case GLUT_KEY_DOWN:
            // Move the camera down along the y-axis
            cameraY -= moveStep;
            break;
        case GLUT_KEY_PAGE_UP:
            // Zoom in
            cameraZ -= zoomStep;
            break;
        case GLUT_KEY_PAGE_DOWN:
            // Zoom out
            cameraZ += zoomStep;
            break;
    }

    // Ensure cameraAngleY stays within the range [0, 360)
    if (cameraAngleY >= 360.0f)
        cameraAngleY -= 360.0f;
    else if (cameraAngleY < 0.0f)
        cameraAngleY += 360.0f;

    // Convert cameraAngleY to radians
    float angleRad = cameraAngleY * 3.14159265358979323846 / 180.0f;

    // Calculate new camera position
    cameraX = 10.0f * sin(angleRad);
    cameraZ = 10.0f * cos(angleRad);

    // Calculate look at point based on camera position
    float lookAtX = -cameraX;
    float lookAtZ = -cameraZ;

    glutPostRedisplay(); // Refresh the display
}



/*--------------------------------------------------------------------------------------------------------------------------------------*/


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode, RGB color, and depth buffer
    glutInitWindowSize(1080, 1080);   // Set the window's initial width & height
    glutInitWindowPosition(500, 500); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    glutSpecialFunc(specialKeys);   // Register callback for special key presses
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
