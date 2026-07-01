#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define IX(i, j) ((i) + (N + 2) * (j))
#define SWAP(x0, x) { float *tmp = x0; x0 = x; x = tmp; }

const int N = 200; // Grid size
const float diff = 0.00000005f;
const float visc = 0.00000001f;
const float dt = 0.1f;
int mouse_down = 0; // 0 = botão solto, 1 = botão pressionado
int mouse_x, mouse_y; // Para armazenar a posição atual do mouse

static float u[(N + 2) * (N + 2)], v[(N + 2) * (N + 2)];
static float u_prev[(N + 2) * (N + 2)], v_prev[(N + 2) * (N + 2)];
static float dens[(N + 2) * (N + 2)], dens_prev[(N + 2) * (N + 2)];

void add_source(int N, float *x, float *s, float dt) {
    for (int i = 0; i < (N + 2) * (N + 2); i++) x[i] += dt * s[i];
}

void set_bnd(int N, int b, float *x) {
    for (int i = 1; i <= N; i++) {
        x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
        x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
    }
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
    x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
    x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void diffuse(int N, int b, float *x, float *x0, float diff, float dt) {
    float a = dt * diff * N * N;
    for (int k = 0; k < 20; k++) {
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                                                x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
            }
        }
        set_bnd(N, b, x);
    }
}

void advect(int N, int b, float *d, float *d0, float *u, float *v, float dt) {
    float dt0 = dt * N;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            float x = i - dt0 * u[IX(i, j)];
            float y = j - dt0 * v[IX(i, j)];

            if (x < 0.5f) x = 0.5f;
            if (x > N + 0.5f) x = N + 0.5f;
            int i0 = (int)x;
            int i1 = i0 + 1;

            if (y < 0.5f) y = 0.5f;
            if (y > N + 0.5f) y = N + 0.5f;
            int j0 = (int)y;
            int j1 = j0 + 1;

            float s1 = x - i0, s0 = 1 - s1;
            float t1 = y - j0, t0 = 1 - t1;

            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                          s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }
    set_bnd(N, b, d);
}

void project(int N, float *u, float *v, float *p, float *div) {
    float h = 1.0f / N;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
                                       v[IX(i, j + 1)] - v[IX(i, j - 1)]);
            p[IX(i, j)] = 0;
        }
    }
    set_bnd(N, 0, div);
    set_bnd(N, 0, p);

    for (int k = 0; k < 20; k++) {
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
                              p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
            }
        }
        set_bnd(N, 0, p);
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            u[IX(i, j)] -= 0.5f * N * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
            v[IX(i, j)] -= 0.5f * N * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
        }
    }
    set_bnd(N, 1, u);
    set_bnd(N, 2, v);
}

void dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt) {
    const float fade = 0.93f; // Ajuste de dispersão mais lenta
    add_source(N, x, x0, dt);
    SWAP(x0, x);
    diffuse(N, 0, x, x0, diff, dt);
    SWAP(x0, x);
    advect(N, 0, x, x0, u, v, dt);

    // Dispersão e suavização nas bordas
    for (int i = 0; i < (N + 2) * (N + 2); i++) {
        x[i] *= fade; // Reduz gradualmente a densidade
        if (x[i] < 0.01f) x[i] = 0.0f; // Evita resíduos de densidade muito pequenos
    }
}


void vel_step(int N, float *u, float *v, float *u0, float *v0, float visc, float dt) {
    add_source(N, u, u0, dt);
    add_source(N, v, v0, dt);
    SWAP(u0, u);
    diffuse(N, 1, u, u0, visc, dt);
    SWAP(v0, v);
    diffuse(N, 2, v, v0, visc, dt);
    project(N, u, v, u0, v0);
    SWAP(u0, u);
    SWAP(v0, v);
    advect(N, 1, u, u0, u0, v0, dt);
    advect(N, 2, v, v0, u0, v0, dt);
    project(N, u, v, u0, v0);
}

void draw_dens() {
    glBegin(GL_QUADS);
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            float x = (float)i / N;
            float y = (float)j / N;
            float d = dens[IX(i, j)];

            // Transição de branco (densidade alta) para preto (densidade baixa)
            glColor3f(d, d * 0.7, d * 0.5); // Gradiente mais suave nas bordas
            glVertex2f(x, y);
            glVertex2f(x + 1.0f / N, y);
            glVertex2f(x + 1.0f / N, y + 1.0f / N);
            glVertex2f(x, y + 1.0f / N);
        }
    }
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_dens();
    glutSwapBuffers();
}

void idle() {
    if (mouse_down) {
        int i = (int)((float)mouse_x / 512 * N);
        int j = (int)((float)(512 - mouse_y) / 512 * N);
        dens[IX(i, j)] += 100.0f;  // Aumenta a densidade para ocupar mais espaço
        u[IX(i, j)] += (rand() % 100 - 50) * 0.2f;      // Aplica força aleatória horizontal
        v[IX(i, j)] += 20.0f;      // Aplica força vertical para simular cogumelo
    }
    vel_step(N, u, v, u_prev, v_prev, visc, dt);
    dens_step(N, dens, dens_prev, u, v, diff, dt);
    glutPostRedisplay();
}


void init() {
    for (int i = 0; i < (N + 2) * (N + 2); i++) {
        u[i] = v[i] = u_prev[i] = v_prev[i] = 0.0f;
        dens[i] = dens_prev[i] = 0.0f;
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouse_down = 1; // Marca como pressionado
            mouse_x = x;    // Atualiza a posição inicial do mouse
            mouse_y = y;
        } else if (state == GLUT_UP) {
            mouse_down = 0; // Marca como solto
        }
    }
}

void motion(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Atomic Explosion Simulation");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
