/*!
@file verilog-ast-walk.c
@brief Contains definitions of functions and structures used to walk the
verilog source tree.
*/

#define VERILOG_LINE_MAX 45

#include <iostream>
#include "verilog-ast-walk.h"
using namespace std;

static int strBuffer = 200;

/*!
@brief Walks over a module declaration, emiting nodes as appropriate.
*/
void walk_port_declaration(
    dot_file                * graph, //!< The graph to emit to.
    dot_node                  parent, //!< parent node of the module.
    ast_port_declaration    * port   //!< The port to walk.
){
    char *params[2];
    params[0] = "Direction:"; 
    params[1] = "Width:";

    char *values[2] = {0, };
    values[0] = (char*) malloc( sizeof(char)*strBuffer );
    values[1] = (char*) malloc( sizeof(char)*strBuffer );

    switch(port -> direction)
    {
        case PORT_INPUT : strcpy(values[0], "input"); break;
        case PORT_OUTPUT: strcpy(values[0], "output");break;
        case PORT_INOUT : strcpy(values[0], "inout");break;
        case PORT_NONE  : strcpy(values[0], "unknown"); break;
        default         : strcpy(values[0], "unknown"); break;
    }

//    cout << "port->range: " << port->range << endl;
    if(port -> range == NULL){
        strcpy(values[1], "1 Bit");
    }
    else{
        sprintf(values[1], "Range:%d-%d", 0, 0);

    }

    for(int P = 0; P < port -> port_names -> items; P++)
    {
        dot_node id = dot_new_node(graph);
        ast_identifier name = (ast_identifier)ast_list_get(port -> port_names, P);

        dot_emit_edge(graph, parent, id);
        dot_emit_record_node(graph, id, name -> identifier,
            params, values, 2);
    }
    free(values[0]);
    free(values[1]);
}

/*!
@brief Handles net declarations.
*/
void walk_net_declaration(
    dot_file                * graph, //!< The graph to emit to.
    dot_node                  parent, //!< parent node of the module.
    ast_type_declaration    * item   //!< The item to walk.
){
    dot_node id = dot_new_node(graph);

    char * params[3];
    params[0] = "Identifier";
    params[1] = "Net Type";
    params[2] = "Width";

    char * values[3];

    char * type;
    switch(item -> type)
    {
        case DECLARE_NET:
            if(item -> net_type == NET_TYPE_WIRE){
                values[1] = "Wire";
            } else {
                values[1] = "Something exotic...";
            }
            break;

        case DECLARE_REG:
            values[1] = "Reg";
            break;

        default:
            break;
    }
    
    if(item -> range == NULL){
        values[2] = "1 Bit";
    } else{
        values[2] = "Bit Range";
    }
            
    ast_identifier net_name = (ast_identifier)ast_list_get(item -> identifiers,0);
    values[0] = net_name->identifier;
            
    dot_emit_edge(graph,parent,id);
    dot_emit_record_node(graph,id,"Declaration",params,values,3);
}
            
/*!
@brief Creates and emits nodes representing a continous assignment.
*/
void walk_continuous_assignment(
    dot_file                * graph, //!< The graph to emit to.
    dot_node                  parent, //!< parent node of the module.
    ast_continuous_assignment * item   //!< The item to walk.
){
    dot_node super = dot_new_node(graph);

    assert(item -> assignments != NULL);

    char * sparams[2];
    sparams[0] = "Delay";
    sparams[1] = "Drive Strength";
    char * svalues[2];
    svalues[0] = " . ";
    svalues[1] = " . ";

    dot_emit_edge(graph, parent,super);
    dot_emit_record_node(graph,super,"Continuous Assignments",
        sparams,svalues,2);

    int i;
    for(i = 0; i < item -> assignments -> items; i ++)
    {
        dot_node said = dot_new_node(graph);

        char * params[2];
        params[0] = "L-Value";
        params[1] = "Assign To";
        char * values[2];
        values[0];
        values[1] = " . ";

        ast_single_assignment * sa = (ast_single_assignment*)ast_list_get(item -> assignments,i);
        values[0] = sa -> lval -> data.identifier -> identifier;
    
        dot_emit_edge(graph,super, said);
        dot_emit_record_node(graph,said,"Single Assignment",
            params,values,2);
    }
}

/*!
@brief Walks over a module declaration, emiting nodes as appropriate.
*/
dot_node walk_module_declaration(
    dot_file                * graph, //!< The graph to emit to.
    dot_node                  parent, //!< parent node of the module.
    ast_module_declaration  * module //!< The module to walk.
){
    dot_node newModule = dot_new_node(graph);

    dot_emit_edge(graph, parent, newModule);
    dot_emit_node(graph,newModule, module -> identifier->identifier);

    dot_node portsParent = dot_new_node(graph);
    dot_emit_edge(graph, newModule, portsParent);
    dot_emit_node(graph, portsParent, "Ports");

    int p;
    for(p = 0; p < module -> module_ports -> items; p ++)
    {
        ast_port_declaration * port = (ast_port_declaration*)ast_list_get(module->module_ports,p);
        walk_port_declaration(graph,portsParent,port);
    }
    return newModule;
}

/*!
@brief Recursively walks the module declaration and instantiation hierarcy.
*/
void walk_module_hierarchy(
    dot_file                * graph, //!< The graph to emit to.
    dot_node                  parent, //!< parent node of the module.
    ast_module_declaration  * module //!< The module to walk.
)
{
    fprintf(stdout, "module %s ( ", module -> identifier->identifier);
   
    int portCnt = module->module_ports->items; 

    for(int i = 0; i < portCnt; i++) {
        ast_port_declaration * port = (ast_port_declaration*)ast_list_get(module->module_ports,i);
        int nameCnt = port->port_names->items;
        for(int j = 0; j < nameCnt; j++) {
            ast_identifier name = (ast_identifier)ast_list_get(port -> port_names, j);
            fprintf(stdout, "%s", name->identifier);
            if( !(i == portCnt-1 && j == nameCnt-1) ) {
                fprintf(stdout, ", ");
            }
        }
    }
    fprintf(stdout, " )\n");
    
    int strSize = 0;
    for(int i = 0; i < portCnt; i++) {
        strSize = 0;
        ast_port_declaration * port = (ast_port_declaration*)ast_list_get(module->module_ports,i);
        
        switch(port -> direction) {
            case PORT_INPUT : fprintf(stdout, "  input "); break;
            case PORT_OUTPUT: fprintf(stdout, "  output ");break;
            case PORT_INOUT : fprintf(stdout, "  inout ");break;
            case PORT_NONE  : fprintf(stdout, "  unknown "); break;
            default         : fprintf(stdout, "  unknown "); break;
        }
        
        if( port->range != NULL) {
            fprintf( stdout, "[");
            strSize += 1; 
            fprintf( stdout, "%s", port->range->upper->primary->value.number->as_bits );
            strSize += strlen( port->range->upper->primary->value.number->as_bits );
            fprintf( stdout, "%s ", port->range->lower->primary->value.number->as_bits );
            strSize += strlen( port->range->lower->primary->value.number->as_bits );
        }

        int nameCnt = port->port_names->items;
        for(int j = 0; j < nameCnt; j++) {
            ast_identifier name = (ast_identifier)ast_list_get(port -> port_names, j);
            fprintf(stdout, "%s", name->identifier);
            if( j != nameCnt-1) {
                fprintf(stdout, ", ");
            }
            strSize += strlen( name->identifier );
            if( strSize > VERILOG_LINE_MAX ) {
                fprintf(stdout, "\n       ");
                strSize = 0;
            }
        }
        fprintf(stdout, ";\n");
    }
    fflush(stdout);
    
    strSize = 0;
    int wireCnt = module->net_declarations->items;
    if( wireCnt > 0 ) {
        fprintf(stdout, "  wire ");
    }
    for(int i = 0; i < wireCnt ; i++) {
        ast_net_declaration* net = (ast_net_declaration*) ast_list_get(module->net_declarations, i);
        fprintf(stdout, "%s", ast_identifier_tostring(net->identifier) );
        
        if( i != wireCnt-1 ) {
            fprintf(stdout, ", ");
        }
        strSize += strlen( ast_identifier_tostring(net->identifier) );
        if( strSize > VERILOG_LINE_MAX ) {
            fprintf(stdout, "\n       ");
            strSize = 0;
        }
    }
    if( wireCnt > 0 ) {
        fprintf(stdout, ";\n");
    }

    for(int i = 0; i < module -> module_instantiations -> items; i ++) {
        ast_module_instantiation * inst = 
                            (ast_module_instantiation*)ast_list_get(module->module_instantiations,i);

        for(int j=0; j < inst->module_instances->items; j++) {
            ast_module_instance* curInst = (ast_module_instance*) 
                ast_list_get( inst->module_instances, j);
            fprintf(stdout, " %s ( ", ast_identifier_tostring(curInst -> instance_identifier));

            int numConnection = curInst->port_connections->items;
            for(int k=0; k<numConnection; k++) {
                ast_port_connection* port = (ast_port_connection*) 
                     ast_list_get( curInst->port_connections, k);
                fprintf(stdout, ".%s(%s)", ast_identifier_tostring( port->port_name ),
                     ast_identifier_tostring(port->expression->primary->value.identifier) ); 
                if( k != numConnection-1 ) {
                    fprintf(stdout, ", ");
                }
            }
            fprintf(stdout, " );\n");
        }
    }
    fprintf(stdout, "endmodule\n");
}

/*!
@brief Recursively decends the syntax tree, emiting the dot graph as it goes.
*/
// Top Level - start from root
void walk_syntax_tree(
    dot_file            * graph, //!< The graph to emit to.
    verilog_source_tree * tree   //!< The source tree to walk.
){

    dot_node root = dot_new_node(graph);

    dot_emit_node(graph,root,"Tree Root");
    
    dot_node mod_hier = dot_new_node(graph);
    dot_emit_edge(graph,root,mod_hier);
    dot_emit_node(graph, mod_hier, "Module Hierarchy");
    
    int m;
    for(m = 0; m < tree -> modules -> items; m ++)
    {
        ast_module_declaration * module = (ast_module_declaration*)ast_list_get(tree->modules,m);
        walk_module_hierarchy(
            graph,
            mod_hier,
            module
        );
    }
}

