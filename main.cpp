#include <iostream>
#include "./Shader/Shader.h"
#include <fstream>
#include <GL/glew.h>
#include <random>
#include <string>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <vector>
#include "./Geometry/box.h"
#include <cassert>
#include "graph.h"


#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))
#define WINDOW_H 600
#define WINDOW_W 600


typedef polygon<point<GLfloat,2>> edge;
typedef glm::vec4 color4;
typedef glm::vec4 point4;

const int NUM_VERTICES = 36;

GLuint program;

std::vector<box<GLfloat>> gboxes;
box<GLfloat> qbox;
bool box_mode = true;
bool intersecting = false;
bool point_mode = false;
bool show_graph = false;

std::vector<point<GLfloat,2>> selected_points;
box<GLfloat> cursor_box;

std::list<point<GLfloat,2>> gpath;
std::list<point<GLfloat,2>> debug_segs;

std::string print_vec(glm::vec4 v)
{
    std::string ret = "";

        ret += std::to_string(v[0]) + std::string(",") + std::to_string(v[1]) + "," + std::to_string(v[2])
            + "," + std::to_string(v[3]) + std::string("\n");
    
        return ret;
}

point4* triangulate(std::vector<box<GLfloat>> boxes, int& size)
{
    point4* ret;
    
    size = boxes.size()*6;
    ret = (point4*) malloc(sizeof(point4)*(boxes.size()*6));
    
    if(size == 0)
    {
        std::cout << "noboxes" << std::endl;
        return NULL;
    }

    int idx = 0;
    for(box<GLfloat> b : boxes)
    {
        ret[idx++] = b[0];
        ret[idx++] = b[1];
        ret[idx++] = b[3];
      
        ret[idx++] = b[1];
        ret[idx++] = b[2];
        ret[idx++] = b[3];
    } 


    return ret;
}

// taken AND learned about from a stack overflow poster
bool SegmentIntersectRectangle(double a_rectangleMinX, double a_rectangleMinY, double a_rectangleMaxX, 
        double a_rectangleMaxY, double a_p1x, double a_p1y, double a_p2x, double a_p2y)
  {

    double minX = a_p1x;
    double maxX = a_p2x;

    if(a_p1x > a_p2x)
    {
      minX = a_p2x;
      maxX = a_p1x;
    }

    if(maxX > a_rectangleMaxX)
    {
      maxX = a_rectangleMaxX;
    }
    if(minX < a_rectangleMinX)
    {
      minX = a_rectangleMinX;
    }
    if(minX > maxX) 
    {
      return false;
    }

    double minY = a_p1y;
    double maxY = a_p2y;
    double dx = a_p2x - a_p1x;

    if(std::fabs(dx) > 0.0000001)
    {
      double a = (a_p2y - a_p1y) / dx;
      double b = a_p1y - a * a_p1x;
      minY = a * minX + b;
      maxY = a * maxX + b;
    }
    if(minY > maxY)
    {
      double tmp = maxY;
      maxY = minY;
      minY = tmp;
    }
    if(maxY > a_rectangleMaxY)
    {
      maxY = a_rectangleMaxY;
    }

    if(minY < a_rectangleMinY)
    {
      minY = a_rectangleMinY;
    }

    if(minY > maxY) 
    {
      return false;
    }
    return true;
}

float dist(float x1,float y1,float x2,float y2)
{
    return std::sqrt(std::pow(x1-x2,2) + std::pow(y1-y2,2));
}

std::list<std::pair<int,GLfloat>> simple_visible_vertices(point<GLfloat,2> p, std::vector<vertex<GLfloat> > verts, std::list<edge> edges)
{
    std::list<std::pair<int,GLfloat>> neighbors;


    for(auto v : verts)
    {
        bool no_intersect = true;
        //check for seg seg intersection here.
        for(auto bx : gboxes)
        {
            if(SegmentIntersectRectangle(bx[0][0]+0.005,bx[0][1]+0.005,bx[1][0]-0.005,bx[2][1]-0.005,p[0],p[1],v.x,v.y))
            {
                no_intersect = false;
                break;
            }
        }
        if(no_intersect)
        {
            neighbors.push_back(std::pair<int,GLfloat>(v.id,dist(p[0],p[1],v.x,v.y)));
            debug_segs.push_back(p);
            debug_segs.push_back(point<GLfloat,2>({v.x,v.y}));
        }
    }
    return neighbors;
}


//constructs a visibility graph and finds path from A to B
std::list<point<GLfloat,2>> vgraph_path(point<GLfloat,2> A, point<GLfloat,2> B)
{
    graph<GLfloat> G;
    int id = 0;
    
    std::vector<vertex<GLfloat> > verts;
    std::list<edge> edges;

    G.add_vertex(A[0],A[1]);
    verts.emplace_back(A[0],A[1],id++);
    
    G.add_vertex(B[0],B[1]);
    verts.emplace_back(B[0],B[1],id++);

    bool first = true;
    int box_id = 0;
    for(auto bx : gboxes)
    {
        for(int i = 0; i < 4; ++i)
        {
            verts.emplace_back(bx[i][0],bx[i][1],id++,box_id);
            G.add_vertex(bx[i][0], bx[i][1]);
        }
        ++box_id;

    }
    //populate the list of edges
    for(int i = 0; i < gboxes.size(); ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            edges.push_back(edge({gboxes[i][j],gboxes[i][j+1]}));
        }
    }

    for(int i = 0; i < verts.size(); ++i)
    {
        G.set_neighbors(verts[i].id, simple_visible_vertices(point<GLfloat,2>({verts[i].x,verts[i].y}),verts,edges));
    }

    return G.path(0,1);
}

void translate(box<GLfloat>& bx, GLfloat factorx, GLfloat factory) 
{
    box<GLfloat> b(bx[0][0]+factorx, bx[1][0]+factorx, bx[0][1]+factory, bx[2][1]+factory);
    b.color = bx.color;
    bx = b;
}

void scale(box<GLfloat>& bx, GLfloat factor)
{
    box<GLfloat> b(bx[0][0]-factor,bx[1][0]+factor,bx[0][1]-factor,bx[2][1]+factor);
    b.color = bx.color;
    bx = b;
}

color4* get_colors(point4* pts, int size, std::vector<box<GLfloat>> bxs)
{
    color4* clrs = (color4*) malloc(sizeof(color4)*size);
    {
        for(int i = 0; i < size; ++i)
        {
            clrs[i] = bxs[i/6].color;
        }
    }

    return clrs;
}

color4 randcolor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.1,0.8);

    return color4(dis(gen),dis(gen),dis(gen),1.0);
}

GLuint theta;

box<GLfloat> box_around_point(point<GLfloat,2> p, float r)
{
    box<GLfloat> ret(p[0]-r, p[0] + r, p[1] - r, p[1] + r); 
    return ret;
}


GLfloat prev_pt[2] = {0,0};
void passiveMotion(int x, int y)
{
    GLfloat curr_x = -1 + ((float)x/(float)WINDOW_W)*2;
    GLfloat curr_y =  1 - ((float)y/(float)WINDOW_H)*2;
    
    GLfloat dx = curr_x - prev_pt[0];
    GLfloat dy = curr_y - prev_pt[1];
   
    translate(cursor_box, dx, dy);

    prev_pt[0] = curr_x;
    prev_pt[1] = curr_y;

    for(int i = 0; i < gboxes.size(); ++i)
    {
        if(cursor_box.intersects(gboxes[i]))
        {
            cursor_box.color = color4(1.0,0.0,0.0,1.0);
            intersecting = true;
            return;
        }
    }
    intersecting = false;
    cursor_box.color = color4(0.0,0.0,0.0,1.0);

}

void display()
{
    int sz = 0;
    std::vector<box<GLfloat>> bxs;

    if(box_mode)
    {
        bxs.push_back(cursor_box);
    }

    bxs.insert(bxs.end(),gboxes.begin(),gboxes.end());

    if(selected_points.size() > 0)
    {
        for(auto p : selected_points)
        {
            box<GLfloat> b(box_around_point(p, 0.005));
            b.color = color4(0.0,0.0,0.0,1.0);
            bxs.push_back(b);
        }
    }
    point4* pts = triangulate(bxs, sz);
    if(sz == 0) 
    {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return; 
    }
    color4* clrs = get_colors(pts,sz,bxs);
    
    glDisable(GL_LIGHTING); 
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glBufferData(GL_ARRAY_BUFFER, sizeof(color4)*sz + sizeof(point4)*sz, NULL, GL_DYNAMIC_DRAW);
	
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4)*sz, pts);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4)*sz, sizeof(color4)*sz, clrs);

	//set up vert arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
   
	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(point4)*sz)));
	
    glDrawArrays(GL_TRIANGLES, 0, sz);
    
    if(gpath.size() > 0)
    { 
        glBegin(GL_LINE_STRIP);
        for(auto p : gpath)
        {
            glColor3f(0.0, 1.0, 0.0);
            glVertex2f(p[0],p[1]);
        }
        
        glColor3f(0.0, 1.0, 0.0);
        glEnd();
    }

    if(show_graph && debug_segs.size() > 0)
    {
        glBegin(GL_LINES);
        
        for(auto p : debug_segs)
        {
            glVertex2f(p[0],p[1]);
        }

        glColor3f(0.0, 1.0, 0.0);
        glEnd();
    }

    free(pts);
    free(clrs);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
        case 'w':
             scale(cursor_box,0.005); break;
        case 's':
             scale(cursor_box,-0.005); break;
        case 'b':
             box_mode = !box_mode;
             break;
        case 'p':
             box_mode = false;
             point_mode = true;
             break;
        case 'l':
             gpath = vgraph_path(selected_points[0], selected_points[1]);
             break;
        case 'g':
             show_graph = !show_graph;
             break;

		case 033:
             selected_points.clear();
             gpath.clear();
             debug_segs.clear();
             break;

		case 'q': case 'Q':
			exit(0);
			break;
	}
}



void mouse(int button, int state, int x, int y)
{
    GLfloat curr_x = -1 + ((float)x/(float)WINDOW_W)*2;
    GLfloat curr_y =  1 - ((float)y/(float)WINDOW_H)*2;
    
    box<GLfloat> bx;
	if(state == GLUT_DOWN)
	{
		switch(button)
		{
			case GLUT_LEFT_BUTTON:
                if(box_mode && !intersecting)
                {   
                    bx = cursor_box;
                    bx.color = randcolor(); 
                    gboxes.emplace_back(bx);
                }
                if(point_mode)
                {
                    if(selected_points.size() < 2)
                    {
                        selected_points.push_back(point<GLfloat,2>({curr_x,curr_y}));
                    }
                }
                break;

		}
	}
}

//series of 2d points, four per line.
std::vector<box<GLfloat>> load_from_file(std::string fname)
{
    std::ifstream box_file(fname);
    std::string line;
    std::string delimeter = ";";
    std::string number;
    std::vector<box<GLfloat>> ret;

    std::vector<float> vals;
    //fmt: f.f.f.f in each line where order is xmin xmax ymin ymax
    while(std::getline(box_file, line))
    {
        size_t i;
        std::cout << line << std::endl;
        while((i = line.find(delimeter)) != std::string::npos)
        {
            number = line.substr(0,i);
            vals.push_back(std::stof(number));
            line.erase(0,i + 1);
        }
        box<GLfloat> bx(vals[0],vals[1],vals[2],vals[3]);
        bx.color = randcolor();
        ret.push_back(bx);
        vals.clear();
    }
    return ret;
}

void idle()
{
	glutPostRedisplay();
}


void init()
{
	
	//load the shaders and use the resulting program
	Practice::Shader *shader = new Practice::Shader("./Shader/fragshader.glslf","./Shader/vertshader.glslv");
	
	program = shader->InitShader();
	glUseProgram(program);
	
	//create and init buffer obj
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	theta = glGetUniformLocation(program, "theta");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.9, 0.9, 0.9, 1.0);
}

int main(int argc, char* argv[])
{
    cursor_box = box<GLfloat>(-0.1,0.1,-0.1,0.1);
    cursor_box.color = color4(0.0,0.0,0.0,1.0);

    std::string fname = "";
    if(argc == 2)
    {
        fname = std::string(argv[1]);
    }

    if(!fname.empty())
    {
        std::vector<box<GLfloat>> tmp = load_from_file(fname);
        gboxes.insert(gboxes.end(),tmp.begin(),tmp.end()); 
    }

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_H,WINDOW_W);
	glutCreateWindow("?????VGRAPH!!!?!?!");
	
	glutInitContextVersion(3,0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glewInit();
	init();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
