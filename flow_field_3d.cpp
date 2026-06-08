#define _USE_MATH_DEFINES
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <GL/gl.h>
#include "Libs/noise/noise.h"
#include "Libs/noise/module/module.h"
#include "Libs/noise/module/perlin.h"
#include <cmath>
#include <iostream>
const int WIDTH = 800;
const int HEIGHT = 800;
const int scl = 10;
float arrx[1000000];
float arry[1000000];
float arrz[1000000];
float prevx[1000000];
float prevy[1000000];
int particles = 1000;
int n = 20;
int size = 200;
int off = 1000;
float r = 0.3;
float map(float v, float x1, float y1, float x2, float y2) {
    return x2 + (y2 - x2) * ((v - x1) / (y1 - x1));
}

int main() {

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "con", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WIDTH, HEIGHT);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -700, 700);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(0.5f);
    float d = 0;
    float t = 0;
    for (int i = 0; i < particles; i++) {
        /*arrx[i] = rand() % (2 * size) - size;
        arry[i] = rand() % (2 * size) - size;
        arrz[i] = rand() % (2 * size) - size;*/
        float alpha = rand();
        float beta = rand();
        float r = rand() % size;
        alpha /= 32767;
        alpha *= 2 * M_PI;
        beta /= 32767;
        beta *= 2 * M_PI;
        arry[i] = r * cos(beta);
        float r2 = r * sin(beta);
        arrx[i] = sin(alpha) * r2;
        arrz[i] = cos(alpha) * r2;

    }
    while (!glfwWindowShouldClose(window)) {
        noise::module::Perlin perlin;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE_LOOP);
        glClear(GL_COLOR_BUFFER_BIT); // Czyœci bufor koloru (ekran)
        // Jeœli u¿ywasz bufora g³êbokoœci, mo¿esz dodaæ: glClear(GL_DEPTH_BUFFER_BIT);

        // Ustawienie koloru t³a na czarny (0.0, 0.0, 0.0 to czarny kolor)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);    

      
        unsigned int vao = 0;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        t += 0.0001;
        d += 0.05;
        glPushMatrix();
        glTranslatef(WIDTH / 2, HEIGHT / 2, 0);
        glRotatef(30, 0.4f, 1.0f, 0.0f);

        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

        glBegin(GL_LINE_LOOP);
        // Front face
        glVertex3f(-size, -size, size);
        glVertex3f(size, -size, size);
        glVertex3f(size, size, size);
        glVertex3f(-size, size, size);
        glEnd();


        glBegin(GL_LINE_LOOP);
        // Back face
        glVertex3f(-size, -size, -size);
        glVertex3f(size, -size, -size);
        glVertex3f(size, size, -size);
        glVertex3f(-size, size, -size);
        glEnd();

        glBegin(GL_LINES);
        // Side edges
        glVertex3f(-size, -size, size);
        glVertex3f(-size, -size, -size);
        glVertex3f(size, -size, size);
        glVertex3f(size, -size, -size);
        glVertex3f(size, size, size);
        glVertex3f(size, size, -size);
        glVertex3f(-size, size, size);
        glVertex3f(-size, size, -size);
        glEnd();
        glColor4f(0.4f, 1.0f, 0.5f, 1.0f);

        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n; j++) {
                for (int k = 0; k <= n; k++) {
                    glColor4f(map(i, 0, n, 0, 0.7) + 0.1, map(j, 0, n, 0, 0.5) + 0.22, map(k, 0, n, 0, 0.8) + 0.1, 1.0f);
                    float x = map(i, 0, n, -size, size);
                    float y = map(j, 0, n, -size, size);
                    float z = map(k, 0, n, -size, size);
                    float alpha = map((perlin.GetValue(x / off, y / off, t)
                        + perlin.GetValue(y / off + 10000, z / off + 10000, t)
                        + perlin.GetValue(z / off + 20000, x / off + 20000, t)), 0, 3, 0, 4 * M_PI);
                    float beta = map((perlin.GetValue(-x / off, -y / off, -t)
                        + perlin.GetValue(-y / off - 10000, -z / off - 10000, -t)
                        + perlin.GetValue(-z / off - 20000, -x / off - 20000, -t)), 0, 3, 0, 4 * M_PI);
                    //  float alpha = map(perlin.GetValue(x / off-z/off, y / off + z / off , t), 0, 1, 0, 2 * M_PI);
                     // float beta = map(perlin.GetValue(x / off + z / off , y / off+200+z/off, t), 0, 1, 0, 2 * M_PI);
                    float r = 10;
                    float y2 = y + r * cos(beta);
                    float r2 = r * sin(beta);
                    float x2 = x + sin(alpha) * r2;
                    float z2 = z + cos(alpha) * r2;
                    glBegin(GL_LINE_LOOP);
                    glVertex3f(x, y, z);
                    glVertex3f(x2, y2, z2);
                    glEnd();
                      }
            }
        }

       /* glPointSize(1);
        for (int w = 0; w < particles; w++) {
            glColor4f(map(arrx[w], -size, size, 0, 0.7) + 0.1, map(arry[w], -size, size, 0, 0.5) + 0.22, map(arrz[w], -size, size, 0, 0.8) + 0.1, 0.01f);

            glBegin(GL_POINTS);
            glVertex3d(arrx[w], arry[w], arrz[w]);
            glEnd();
            float x = arrx[w];
            float y = arry[w];
            float z = arrz[w];

            float alpha = map((perlin.GetValue(x / off, y / off, t)
                + perlin.GetValue(y / off + 10000, z / off + 10000, t)
                + perlin.GetValue(z / off + 20000, x / off + 20000, t)), 0, 3, 0, 4 * M_PI);
            float beta = map((perlin.GetValue(-x / off, -y / off, -t)
                + perlin.GetValue(-y / off - 10000, -z / off - 10000, -t)
                + perlin.GetValue(-z / off - 20000, -x / off - 20000, -t)), 0, 3, 0, 4 * M_PI);
            arry[w] += r * cos(beta);
            float r2 = r * sin(beta);
            arrx[w] += sin(alpha) * r2;
            arrz[w] += cos(alpha) * r2;
             if (arrx[w] * arrx[w] + arry[w] * arry[w] + arrz[w] * arrz[w] > size * size) {
                float alpha = rand();
                float beta = rand();
                float r = rand() % size;
                alpha /= 32767;
                alpha *= 2 * M_PI;
                beta /= 32767;
                beta *= 2 * M_PI;
                arry[w] = r * cos(beta);
                float r2 = r * sin(beta);
                arrx[w] = sin(alpha) * r2;
                arrz[w] = cos(alpha) * r2;
                arrx[w] *= -1;
                arry[w] *= -1;

                arrz[w] *= -1;
                if (arrx[w] > 0) {
                    arrx[w] -= 15;
                }
                else {
                    arrx[w] += 15;
                }
                if (arry[w] > 0) {
                    arry[w] -= 15;
                }
                else {
                    arry[w] += 15;
                }
                if (arrz[w] > 0) {
                    arrz[w] -= 15;
                }
                else {
                    arrz[w] += 15;
                }
            }

        }
        */
        glPopMatrix();
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}