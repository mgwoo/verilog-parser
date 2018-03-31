/*!
@file verilog-dot-emit.c
@brief Contains definitions of functions and structures used to emit dot file
data.
*/

#include <iostream>
using namespace std;
#include "verilog-dot-emit.h"

/*!
@details
Creates and returns a new dot file object which can be written to.
This function also writes the preamble to the graph. After calling dot_file_new
one must also call dot_file_close, which writes the postamble, and finishes
the file.
*/
dot_file * dot_file_new(FILE * writable)
{
    dot_file * tr = (dot_file*) calloc(1,sizeof(dot_file));

    tr -> node_count = 0;
    tr -> file       = writable;

    fprintf(tr -> file, "\ndigraph verilog_dot_graph{\n");
    fprintf(tr -> file, "graph [pad=0,ranksep=3,nodesep=5];\n");

    return  tr;
}

/*!
@brief Finishes off the output file, writing any extra syntax.
@post The output file is closed, and is a valid .dot file, ready to be
viewed.
*/
void dot_file_finish(dot_file * graph)
{
    fprintf(stdout, "}\n");
    fclose(stdout);
}

/*!
@brief Emits a simple node to the graph, with a given ID and label.
*/
void dot_emit_node(dot_file * graph, dot_node id, char * label)
{
//    fprintf(stdout,"\t%d [shape=box,label=\"%s\"];\n",id,label);
    fprintf(stdout,"\t\tNode[%d]: %s\n",id,label);
}


/*!
@brief Emits a record node to the graph, with a given ID, label and set of
characteristics.
*/
void dot_emit_record_node(
    dot_file * graph, 
    dot_node id, 
    char * label,
    char ** parameters, 
    char ** values, 
    int num_params
){
    //fprintf(stdout,"\t%d [shape=none,label=<<table border='0' cellborder='1' cellspacing='0' cellpadding='4'><tr><td colspan='%d' bgcolor='black' ><font color='white'><b>%s</b></font></td></tr>\n",id,num_params,label);
    fprintf(stdout,"\t\tNode[%d]:%s (num:%d)\n",id,label, num_params);
//    fprintf(stdout,"\t%s\n",label);

    // num_params info
    for(int i = 0; i < num_params; i++){
        fprintf(stdout, "\t\t\t%s -> %s\n",parameters[i], values[i]);

//        fprintf(graph->file,"<tr>");
//        fprintf(stdout, "<td><b>%s</b></td>",parameters[i]);
//        fprintf(stdout, "<td>%s</td>",values[i]);
//        fprintf(graph->file,"</tr>");
    }
}

/*!
@brief Emits an edge between two pre-defined nodes.
*/
void dot_emit_edge(dot_file * graph, dot_node id1, dot_node id2)
{
    fprintf(stdout, "\tEdge: %d -> %d\n", id1,id2);
}

/*!
@brief Emits a directed edge between two pre-defined nodes.
*/
void dot_emit_directed_edge(dot_file * graph, dot_node id1, dot_node id2)
{
    fprintf(stdout, "\tDEdge: %d -> %d\n", id1,id2);
}

void dot_emit_range( dot_file* graph, dot_range* range) {
}

/*!
@brief Generates a new node ID for the graph and returns it.
*/
dot_node dot_new_node(dot_file * graph)
{
    graph -> node_count ++;
    return graph -> node_count;
}
