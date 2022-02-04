# DCEL
The doubly connected edge list (DCEL), also known as half-edge data structure, is a data structure to represent an
embedding of a planar graph in the plane, and polytopes in 3D. This data structure provides efficient[quantify]
manipulation of the topological information associated with the objects in question (vertices, edges, faces). It is
used in many algorithms of computational geometry to handle polygonal subdivisions of the plane, commonly
called planar straight-line graphs (PSLG). For example, a Voronoi diagram is commonly represented by a DCEL
inside a bounding box.

Assumptions Taken :-
- Edges to be made in Clockwise direction

- Split.txt file should include formatting like
  Split x y
  Id: a b c
  where x and y are edges to be splitted, a is Id number , b and c is the x and y componenet of point to be checked.

- I have checked for some cases like 2 3, 3 2, 1 3, and some multiple splits and it is working correct.

- I am iterating edges from new edge formed while splitting to it for different faces so output will be as per the edges we iterate.

- For printing output of new function i.e. To get Face number, my printing format is:-
Id: ID Face_number
where ID is the id given to the point and Face_number is the face number on which the point is present.

To compile
-make
-./dcel input.in split.txt output.out 
