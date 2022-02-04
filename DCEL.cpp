#include <bits/stdc++.h>

#include <iostream>
#include <fstream>
using namespace std;

class Vertex
{
public:
	double x, y, i;
};

class Face
{
public:
	int face_index;
	class HalfEdge *incident_edge;
};

class HalfEdge
{
public:
	HalfEdge *next_edge, *prev_edge;
	Vertex *start_v, *end_v;
	Face *incident_face;
	int edge_index;
	HalfEdge *twin;
};

class DCEL
{
public:
	vector<Vertex> vertices;
	HalfEdge *halfedge_start, *halfedge_end;
	vector<Face> face;
	int no_edges;
};

class HalfEdge *make_halfedge(Vertex *st, Vertex *e, HalfEdge **start_edge, HalfEdge *end_edge, Face *face)
{
	// Make an edge
	HalfEdge *edge = new HalfEdge;

	HalfEdge *ending = *start_edge;

	edge->start_v = st;
	edge->end_v = e;

	edge->next_edge = NULL;
	edge->incident_face = face;

	//First edge
	if (*start_edge == NULL)
	{
		edge->prev_edge = NULL;
		edge->edge_index = 0;
		(*start_edge) = edge;
		return edge;
	}
	// Code for 2nd edge and onwards which will be connected
	edge->edge_index = end_edge->edge_index + 1;
	while (ending->next_edge != NULL)
	{
		ending = ending->next_edge;
	}
	ending->next_edge = edge;
	edge->prev_edge = ending;
	return edge;
}

//Make mid of 2 vertices
class Vertex *mid(Vertex *a, Vertex *b)
{
	Vertex *mid = new Vertex;
	mid->x = (a->x + b->x) / 2;
	mid->y = (a->y + b->y) / 2;

	return mid;
}

// Make and edge with the help of 2 vertices(mids) and 2 edges respectively of the vertices
HalfEdge *make_edge(Vertex *v1, Vertex *v2, HalfEdge **e1, HalfEdge *e2)
{
	HalfEdge *new_edge = new HalfEdge;
	HalfEdge *ending = *e1;
	new_edge->start_v = v1;
	new_edge->end_v = v2;
	new_edge->next_edge = e2;
	new_edge->prev_edge = *e1;
	new_edge->incident_face = e2->incident_face;

	return new_edge;
}

//Gives the info of kth particular edge
HalfEdge *edge_info(HalfEdge *e, int k)
{
	HalfEdge *the_edge = new HalfEdge;
	for (int i = 0; i < k; i++)
	{
		the_edge = e->next_edge;
	}
	return the_edge;
}

// making of new edges by shrinking the 2 edges to small ones and connecting them
HalfEdge *New_edges_t(Face *new_face, HalfEdge *new_twin, Vertex *v1, Vertex *v2)
{
	HalfEdge *new_edge_1 = new HalfEdge;
	HalfEdge *new_edge_2 = new HalfEdge;

	// updating new edge 1
	new_edge_1->start_v = new_twin->end_v;
	new_edge_1->end_v = v1;
	new_edge_1->edge_index = new_twin->edge_index + 1;

	//Updating new edge 2
	new_edge_2->start_v = v2;
	new_edge_2->end_v = new_twin->start_v;
	new_edge_2->edge_index = new_edge_1->edge_index + 1;

	//Updating other edges w.r.t twin
	new_twin->next_edge = new_edge_1;
	new_twin->prev_edge = new_edge_2;

	new_edge_1->next_edge = new_edge_2;
	new_edge_1->prev_edge = new_twin;

	new_edge_2->next_edge = new_twin;
	new_edge_2->prev_edge = new_edge_1;
	//Updating face
	new_face->incident_edge = new_twin;

	return new_twin;
}
//Making new edges in case where vertices are on opposite edges
HalfEdge *New_edges_r(Face *new_face, HalfEdge *new_twin, Vertex *v1, Vertex *v2, HalfEdge *e1_next, HalfEdge *e2_prev)
{
	HalfEdge *new_edge_1 = new HalfEdge;
	HalfEdge *new_edge_2 = new HalfEdge;
	//Updating new edge 1
	new_edge_1->start_v = new_twin->end_v;
	new_edge_1->end_v = v1;
	new_edge_1->edge_index = new_twin->edge_index + 1;
	//Updating new edge 2
	new_edge_2->start_v = v2;
	new_edge_2->end_v = new_twin->start_v;
	new_edge_2->edge_index = new_edge_1->edge_index + 1;

	new_twin->next_edge = new_edge_1;
	new_twin->prev_edge = new_edge_2;

	new_edge_1->next_edge = e1_next;
	new_edge_1->prev_edge = new_twin;

	new_edge_2->next_edge = new_twin;
	new_edge_2->prev_edge = e1_next;

	e1_next->next_edge = new_edge_2;
	e1_next->prev_edge = new_edge_1;

	new_face->incident_edge = new_twin;

	return new_twin;
}

//Making new edges in case of reverse split
HalfEdge *New_edges_r_t(Face *new_face, HalfEdge *new_twin, Vertex *v1, Vertex *v2, HalfEdge *e1_next, HalfEdge *e2_prev)
{
	HalfEdge *new_edge_1 = new HalfEdge;
	HalfEdge *new_edge_2 = new HalfEdge;
	//Updating new edges
	new_edge_1->start_v = new_twin->end_v;
	new_edge_1->end_v = v1;
	new_edge_1->edge_index = new_twin->edge_index + 1;

	new_edge_2->start_v = v2;
	new_edge_2->end_v = new_twin->start_v;
	new_edge_2->edge_index = new_edge_1->edge_index + 1;

	new_twin->next_edge = new_edge_1;
	new_twin->prev_edge = new_edge_2;

	new_edge_1->next_edge = e1_next;
	new_edge_1->prev_edge = new_twin;

	new_edge_2->next_edge = new_twin;
	new_edge_2->prev_edge = e2_prev;

	e1_next->next_edge = e2_prev;
	e1_next->prev_edge = new_edge_1;

	e2_prev->next_edge = new_edge_2;
	e2_prev->prev_edge = e1_next;

	new_face->incident_edge = new_twin;

	return new_twin;
}
//Splitting of edges
HalfEdge *split(HalfEdge *e1, HalfEdge *e2, DCEL *dcel, int adj)
{

	Vertex *v1 = new Vertex;
	v1 = mid(e1->start_v, e1->end_v);
	v1->i = dcel->vertices.size();

	Vertex *v2 = new Vertex;
	v2 = mid(e2->start_v, e2->end_v);
	v2->i = dcel->vertices.size() + 1;

	Vertex *store_dest_1 = new Vertex;
	store_dest_1 = e1->end_v;
	Vertex *store_dest_2 = new Vertex;
	store_dest_2 = e2->start_v;
	HalfEdge *e1_next = new HalfEdge;

	e1_next = e1->next_edge;

	HalfEdge *e2_prev = new HalfEdge;
	e2_prev = e2->prev_edge;

	HalfEdge *new_edge = new HalfEdge;
	new_edge = make_edge(v1, v2, &e1, e2);
	new_edge->edge_index = dcel->no_edges;
	e1->end_v = v1;
	e2->start_v = v2;
	e1->next_edge = new_edge;
	e2->prev_edge = new_edge;
	new_edge->next_edge = e2;
	new_edge->prev_edge = e1;

	Face *new_face = new Face;
	new_face->face_index = dcel->face.size();
	new_face->incident_edge = NULL;

	HalfEdge *new_twin = new HalfEdge;
	new_edge->twin = new_twin;
	new_twin->twin = new_edge;

	new_twin->edge_index = new_edge->edge_index;
	new_twin->start_v = new_edge->end_v;
	new_twin->end_v = new_edge->start_v;
	//Three different cases
	if (adj == 0)
	{
		new_twin = New_edges_t(new_face, new_twin, store_dest_1, store_dest_2);
		e1->prev_edge = e2;
	}
	else if (adj == -1)
	{
		e1->prev_edge = e2;
		new_twin = New_edges_r_t(new_face, new_twin, store_dest_1, store_dest_2, e1_next, e2_prev);
	}
	else
	{
		e1->prev_edge = e2->next_edge;
		new_twin = New_edges_r(new_face, new_twin, store_dest_1, store_dest_2, e1_next, e2_prev);
	}
	dcel->no_edges += 3;

	dcel->face[0].incident_edge = new_edge;

	Face f1;
	f1.face_index = 0;
	f1.incident_edge = new_edge;
	dcel->face[0] = f1;

	dcel->face.push_back(*new_face);

	dcel->vertices.push_back(*v1);
	dcel->vertices.push_back(*v2);

	new_twin = dcel->halfedge_start;

	return new_edge;
}

// To see if these two point are on one side of line or not
// One centroid , other the given point
int Sign(Vertex *v1, Vertex *v2, Vertex *t, Vertex *p)
{
	double slope;
	slope = (v2->y - v1->y) / (v2->x - v1->x);
	double x = t->y - v2->y - slope * (t->x - v2->x);
	double y = p->y - v2->y - slope * (p->x - v2->x);

	// Same side of line
	if (x * y >= 0)
		return 1;
	// opposite side of line
	else
		return -1;
}
// To fine Centroid
Vertex *Centroid(Face *face)
{
	HalfEdge *e = new HalfEdge;
	e = face->incident_edge;
	int n = 0;
	Vertex *t = new Vertex;
	t->x = 0;
	t->y = 0;
	do
	{
		n++;
		t->x += e->start_v->x;
		t->y += e->start_v->y;
		e = e->next_edge;
	} while (e != face->incident_edge);
	Vertex *c = new Vertex;
	c->x = t->x / n;
	c->y = t->y / n;
	return c;
}
// To find the face number where the point is
int Face_num(Vertex *p, DCEL *dcel)
{
	int index = -1;
	// Iterating through all faces
	for (int i = 0; i < dcel->face.size(); i++)
	{
		HalfEdge *edge = dcel->face[i].incident_edge;
		Vertex *centroid = Centroid(&(dcel->face[i]));
		//Iterating through all edges of that face and checking th conditions
		do
		{
			if (Sign(edge->start_v, edge->end_v, centroid, p) == 1)
			{
				index = i;
			}
			else
			{
				break;
			}
			edge = edge->next_edge;

		} while (edge != dcel->face[i].incident_edge);

		if (index > -1)
			return index;
	}
	return -1;
}
void print_output(DCEL *d, HalfEdge **HalfEdge_start, ofstream &f)
{
	HalfEdge *ptr = new HalfEdge;
	ptr = *HalfEdge_start;
	f << "Edges:" << endl;
	for (int j = 0; j < d->no_edges; j++)
	{
		f << j << endl;
	}
	f << "Halfedges:" << endl;
	do
	{
		f << "startVertexIndex=" << ptr->start_v->i << " "
		  << "endVertexIndex=" << ptr->end_v->i << " "
		  << "nextEdge=" << ptr->next_edge->edge_index << " "
		  << "previousEdge=" << ptr->prev_edge->edge_index << " "
		  << "faceIndex=" << ptr->incident_face->face_index << " "
		  << "edgeIndex=" << ptr->edge_index << endl;
		ptr = ptr->next_edge;

	} while (ptr != *HalfEdge_start);

	f << "Faces:" << endl;
	f << "FacesIndex:";
	f << ptr->incident_face->face_index;

	ptr = *HalfEdge_start;

	f << "Edges ";
	do
	{
		if (ptr->next_edge == *HalfEdge_start)
			f << ptr->edge_index;
		else
			f << ptr->edge_index << "->";
		ptr = ptr->next_edge;
	} while (ptr != *HalfEdge_start);
}

void print_output_split(HalfEdge **HalfEdge_start, DCEL *d, ofstream &f)
{
	f << "Edges:" << endl;
	for (int j = 0; j < d->no_edges; j++)
	{
		f << j << endl;
	}
	f << "Halfedges:" << endl;
	for (int i = 0; i < d->face.size(); i++)
	{
		HalfEdge *ptr = d->face[i].incident_edge;

		do
		{
			f << "startVertexIndex=" << ptr->start_v->i << " "
			  << "endVertexIndex=" << ptr->end_v->i << " "
			  << "nextEdge=" << ptr->next_edge->edge_index << " "
			  << "previousEdge=" << ptr->prev_edge->edge_index << " "
			  << "faceIndex=" << i << " "
			  << "edgeIndex=" << ptr->edge_index << endl;
			ptr = ptr->next_edge;
		} while (ptr != d->face[i].incident_edge);
	}
	f << "Faces:" << endl;

	for (int i = 0; i < d->face.size(); i++)
	{
		HalfEdge *edge = d->face[i].incident_edge;

		if (edge != NULL)
		{

			f << "FaceIndex:" << d->face[i].face_index << " "
			  << "Edges: ";
			do
			{
				if (edge->next_edge != d->face[i].incident_edge)
					f << edge->edge_index << "->";
				else
				{
					f << edge->edge_index << endl;
				}
				edge = edge->next_edge;

			} while (edge != d->face[i].incident_edge);
		}
	}
}

int main(int argc, char *argcv[])

{
	int x, y;
	ifstream f;
	f.open(argcv[1]);

	DCEL dcel; //initialising dcel
	dcel.halfedge_start = NULL;
	dcel.halfedge_end = NULL;
	dcel.no_edges = 0;

	Face fc;
	fc.face_index = 0;
	fc.incident_edge = dcel.halfedge_start;
	dcel.face.push_back(fc);

	for (int j = 0; j < 4; j++)
	{
		//Storing vertices in dcel and declaring index of vertixes
		Vertex t1;
		f >> t1.x >> t1.y;
		t1.i = j;
		dcel.vertices.push_back(t1);

		dcel.vertices[j].i = j;
	}
	for (int j = 0; j < 3; j++)
	{
		//Making Halfedges
		//Pointing edges to next one. edge 1 to 2, edge 2 to 3, edge 3 to 4 that is clockwise direction
		dcel.halfedge_end = make_halfedge(&dcel.vertices[j], &dcel.vertices[j + 1], &dcel.halfedge_start, dcel.halfedge_end, &dcel.face[0]);
		dcel.no_edges++;
	}
	dcel.no_edges++;
	//pointing edge 4 back to 1 as we are shape of polygon we are making is a square
	dcel.halfedge_end = make_halfedge(&dcel.vertices[3], &dcel.vertices[0], &dcel.halfedge_start, dcel.halfedge_end, &dcel.face[0]);
	dcel.halfedge_start->prev_edge = dcel.halfedge_end;
	dcel.halfedge_end->next_edge = dcel.halfedge_start;

	ofstream f_o;

	f_o.open(argcv[3]);
	print_output(&dcel, &dcel.halfedge_start, f_o);

	HalfEdge *e1 = new HalfEdge;
	HalfEdge *e2 = new HalfEdge;
	HalfEdge *e = new HalfEdge;
	HalfEdge *e_prev = new HalfEdge;
	HalfEdge *e_next = new HalfEdge;
	e = dcel.halfedge_start;

	ifstream f1;
	f1.open(argcv[2]);
	string split_id = "";
	f1 >> split_id;

	while (f1.eof() == false)
	{
		if (split_id == "Split")
		{
			int v_1, v_2, adj = 1;
			f1 >> v_1 >> v_2;

			if ((v_1 - v_2) == -1 || v_1 - v_2 == 3)
			{
				adj = 0;
			}
			else if (v_1 - v_2 == 1)
			{
				adj = -1;
			}
			e_prev = edge_info(dcel.halfedge_start, v_1 - 1);
			e_next = edge_info(dcel.halfedge_start, v_1 + 1);
			for (int j = 0; j < v_1; j++)
			{
				e = e->next_edge;
			}
			e1 = e;
			e1->prev_edge = e_prev;
			e1->next_edge = e->next_edge;

			e = dcel.halfedge_start;
			e_prev = edge_info(dcel.halfedge_start, v_2 - 1);
			e_next = edge_info(dcel.halfedge_start, v_2 + 1);
			for (int j = 0; j < v_2; j++)
			{
				e = e->next_edge;
			}
			e2 = e;
			e2->prev_edge = e_prev;

			HalfEdge *t = new HalfEdge;
			t = dcel.halfedge_start;

			HalfEdge *New_edge = split(e1, e2, &dcel, adj);
			f_o << endl;
			f_o << endl;
			f_o << "//After split";
			f_o << endl;
			f_o << endl;
			print_output_split(&dcel.halfedge_start, &dcel, f_o);
		}
		if (split_id == "Id:")
		{
			Vertex p;
			int Id;
			f1 >> Id >> p.x >> p.y;
			int x = Face_num(&p, &dcel);
			f_o << "Id: " << Id << " " << x << endl;
		}
		f1 >> split_id;
	}
}
//For splitting an edge,I have first found out the mid points,then I have created the split edge.
// Now i have passed the twin of the split edge and created a new face for it.Each split results in the creation of a new face and two new edges.
// I have made this into a doubly linked list.
// To find the face on which the point lies, I iterated all the edges by their faces and with the help of centroid, I printed the face number on which the point belongs to
