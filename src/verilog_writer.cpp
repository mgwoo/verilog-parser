/*!
@file verilog-ast-walk.c
@brief Contains definitions of functions and structures used to walk the
verilog source tree.
*/

#define VERILOG_LINE_MAX 45
#define CUSTOM_FPRINTF(fmt, ...) {if(fmt) {fprintf(fmt, ##__VA_ARGS__);}}


#include <iostream>
#include "verilog_writer.h"
using namespace std;


#define GetVariableName( varname ) ( #varname )
#define RaiseErrorForNull( varname )                            \
    if( !varname ) {                                            \
        cout << string( "ERROR** : parameter " )                \
        + string(GetVariableName(varname))                      \
        + string(" is Empty") << endl;                          \
        exit(1);                                                \
    }

NAMESPACE_VERILOG_BEGIN

static int strBuffer = 200;

int PrintIdentifier( FILE* fout, ast_identifier& identifier ) {
    RaiseErrorForNull( identifier );
    CUSTOM_FPRINTF(fout, "%s", ast_identifier_tostring( identifier ) );
    return strlen(ast_identifier_tostring(identifier));
}

int PrintRangeNumber( FILE* fout, ast_number* number ) {
    CUSTOM_FPRINTF(fout,  "%s", number->as_bits ); 
}

// this Part 'Must be' updated later
int PrintNumber( FILE* fout, ast_number* number ) {
    RaiseErrorForNull( number );
    int cnt = 0;

    switch( number->representation ) {
        // currently, only REP_BITS is used!!
        case REP_BITS:
            CUSTOM_FPRINTF( fout, "%d'", strlen(number->as_bits) );
            switch( number->base ) {
                case BASE_BINARY: CUSTOM_FPRINTF( fout, "b" ); break;
                case BASE_OCTAL: CUSTOM_FPRINTF( fout, "o" ); break;
                case BASE_HEX: CUSTOM_FPRINTF( fout, "h" ); break;

                case BASE_DECIMAL:
                default: CUSTOM_FPRINTF( fout, "d"); break;
            }
            CUSTOM_FPRINTF( fout, "%s", number->as_bits );   
            cnt += strlen(number->as_bits);
            break;
        // not used
        case REP_INTEGER: 
            CUSTOM_FPRINTF( fout, "%d", number->as_int );
            break;
        case REP_FLOAT: 
            CUSTOM_FPRINTF( fout, "%f", number->as_float );
            break;
    }
    cnt += 2;
    return cnt;
}

int PrintExpression( FILE* fout, ast_expression* expression ) {
    if( !expression ) { return 0; }
    RaiseErrorForNull( expression );
    
    char * tr;
    char * lhs;
    char * rhs;
    char * pri;
    char * cond;
    char * mid;
    char * op;
    size_t len = 0;

    switch(expression -> type) {
        case PRIMARY_EXPRESSION:
        case MODULE_PATH_PRIMARY_EXPRESSION:
            len += PrintPrimary(fout, expression -> primary);
            // added by mgwoo
            if(expression->right) {
                rhs = ast_expression_tostring(expression->right);
                op = strdup("[");
                strcat(op, rhs);
                CUSTOM_FPRINTF(fout, "%s", op);
                free(rhs);
                len += strlen(op);
            }
            break;
        case STRING_EXPRESSION:
            tr = ast_strdup(expression -> string);
            len = strlen(tr);
            free(tr);
            break;
            /*
             * to be supported later
        case UNARY_EXPRESSION:  
        case MODULE_PATH_UNARY_EXPRESSION:
            pri = ast_primary_tostring(expression -> primary);
            op  = ast_operator_tostring(expression -> operation);
            tr = (char*)ast_calloc(strlen(pri)+5,sizeof(char));
            strcat(tr,"(");
            strcat(tr, op); 
            strcat(tr,pri);
            strcat(tr,")");
            break;
        case BINARY_EXPRESSION:
        case MODULE_PATH_BINARY_EXPRESSION:
            lhs = ast_expression_tostring(expression -> left);
            rhs = ast_expression_tostring(expression -> right);
            op  = ast_operator_tostring(expression -> operation);
            len =5+strlen(lhs)+ strlen(rhs);
            tr = (char*)ast_calloc(len,sizeof(char));
            strcat(tr,"(");
            strcat(tr,lhs);
            strcat(tr, op); 
            strcat(tr,rhs);
            strcat(tr,")");
            break;
        case RANGE_EXPRESSION_UP_DOWN:
            lhs = ast_expression_tostring(expression -> left);
            rhs = ast_expression_tostring(expression -> right);
            len =3+strlen(lhs)+ strlen(rhs);
            tr = (char*)ast_calloc(len,sizeof(char));
            strcat(tr,lhs);
            strcat(tr,":");
            strcat(tr,rhs);
            break;
        case RANGE_EXPRESSION_INDEX:
            tr = ast_expression_tostring(expression -> left);
            break;
        case MODULE_PATH_MINTYPMAX_EXPRESSION:
        case MINTYPMAX_EXPRESSION: 
            lhs = ast_expression_tostring(expression -> left);
            rhs = ast_expression_tostring(expression -> right);
            mid = ast_expression_tostring(expression -> aux);
            len = 3 +
                  strlen(lhs) + 
                  strlen(rhs) + 
                  strlen(mid);
            tr = (char*)ast_calloc(len,sizeof(char));
            strcat(tr,lhs);
            strcat(tr,":");
            strcat(tr,mid);
            strcat(tr,":");
            strcat(tr,rhs);
            break;
        case CONDITIONAL_EXPRESSION: 
        case MODULE_PATH_CONDITIONAL_EXPRESSION:
            lhs = ast_expression_tostring(expression -> left);
            rhs = ast_expression_tostring(expression -> right);
            cond= ast_expression_tostring(expression -> aux);
            len = 3 +
                  strlen(lhs) + 
                  strlen(rhs) + 
                  strlen(cond);
            tr = (char*)ast_calloc(len,sizeof(char));
            strcat(tr,cond);
            strcat(tr,"?");
            strcat(tr,lhs);
            strcat(tr,":");
            strcat(tr,rhs);
            break;
        default:
            printf("ERROR: Expression type to string not supported. %d of %s",
                __LINE__,__FILE__);
            tr = "<unsupported>";
            break;
            */
    }
    return len;
    
    /*
    int cnt = 0;
    cout << "expression type: " << expression->type << endl;
    if( expression->left ) {
        cout << "LEFT" << endl;
        cnt += PrintExpression( fout, expression->left );
    }
    switch( expression -> type ) {
        case PRIMARY_EXPRESSION:
            cnt += PrintPrimary( fout, expression->primary );
            break;
        // to be updated later
    }
    if( expression -> attributes ) {
        exit(1);
    }
    if( expression->right ) {
        cout << "RIGHT" << endl;
        cnt += PrintExpression( fout, expression->right );
    }
    if( expression->aux) {
        cout << "AUX" << endl;
        cnt += PrintExpression( fout, expression->aux ) ;
    
    }
    return cnt;
    */
}

int PrintConcatenation( FILE* fout, ast_concatenation* concatenation ) {
    RaiseErrorForNull( concatenation );
    int cnt = 0;
//    cout << "concatenation type: " << concatenation->type << endl;
    switch( concatenation->type ) {
        case CONCATENATION_EXPRESSION:
        case CONCATENATION_CONSTANT_EXPRESSION:
            cnt = 0;
//            cout << "concatenation size: " << concatenation->items->items << endl;
            cnt += PrintExpression( fout, concatenation->repeat );
            CUSTOM_FPRINTF( fout, ", " );
            cnt += 2;
            for(int i=0; i< concatenation->items->items; i++) {
                ast_expression* expr = (ast_expression*)ast_list_get( concatenation->items, i );
                cnt += PrintExpression( fout, expr );

                if( i != concatenation->items->items -1 ) {
                    CUSTOM_FPRINTF( fout, ", " );
                    cnt += 2; 
                }
            }
            return cnt;
        case CONCATENATION_NET:
        case CONCATENATION_VARIABLE:
        case CONCATENATION_MODULE_PATH:
            cnt = 0;
            for( int i=0; i< concatenation->items->items; i++) {
                ast_identifier identifier = (ast_identifier)ast_list_get( concatenation->items, i ); 
                fprintf(fout,"%s", ast_identifier_tostring(identifier) );
                cnt += strlen( ast_identifier_tostring(identifier) );
            }
            return cnt; 
    }
}

int PrintPrimary( FILE* fout, ast_primary* primary) {
    RaiseErrorForNull( primary );
    
    int cnt = 0;
    char* tmpchar = NULL;
//    cout << "primary type: " << primary->value_type << endl;
    switch( primary->value_type ) {
        case PRIMARY_NUMBER:        
            return PrintNumber( fout, primary->value.number );
        case PRIMARY_IDENTIFIER:    
            return PrintIdentifier( fout, primary->value.identifier );  
        case PRIMARY_CONCATENATION:
            cnt = 0;
            CUSTOM_FPRINTF( fout, "{" ); 
            cnt += PrintConcatenation( fout, primary->value.concatenation );
            CUSTOM_FPRINTF( fout, "}" );
            cnt += 2; 
            return cnt;
        case PRIMARY_FUNCTION_CALL:
            return PrintIdentifier( fout, primary->value.function_call -> function);  
        case PRIMARY_MINMAX_EXP:
            tmpchar = ast_expression_tostring( primary->value.minmax );
            CUSTOM_FPRINTF( fout, "%s", tmpchar );
            cnt = strlen(tmpchar);
            free(tmpchar);
            return cnt;
        case PRIMARY_MACRO_USAGE:
        default: break;
    }
}

/*!
@brief Recursively walks the module declaration and instantiation hierarcy.
*/
void PrintModule ( FILE* fout, ast_module_declaration  * module ) {

    // print module title
//    FILE* fout = stdout;
    CUSTOM_FPRINTF(fout, "module %s ( ", module -> identifier->identifier);
   
    int portCnt = module->module_ports->items; 

    for(int i = 0; i < portCnt; i++) {
        ast_port_declaration * port = (ast_port_declaration*)ast_list_get(module->module_ports,i);
        int nameCnt = port->port_names->items;
        for(int j = 0; j < nameCnt; j++) {
            ast_identifier name = (ast_identifier)ast_list_get(port -> port_names, j);
            PrintIdentifier( fout, name );
            if( !(i == portCnt-1 && j == nameCnt-1) ) {
                CUSTOM_FPRINTF(fout, ", ");
            }
        }
    }
    CUSTOM_FPRINTF(fout, " );\n");
   
    // print port information 
    int strSize = 0;
    for(int i = 0; i < portCnt; i++) {
        strSize = 0;
        ast_port_declaration * port = (ast_port_declaration*)ast_list_get(module->module_ports,i);
        
        switch(port -> direction) {
            case PORT_INPUT : CUSTOM_FPRINTF(fout, "  input "); break;
            case PORT_OUTPUT: CUSTOM_FPRINTF(fout, "  output ");break;
            case PORT_INOUT : CUSTOM_FPRINTF(fout, "  inout ");break;
            case PORT_NONE  : CUSTOM_FPRINTF(fout, "  unknown "); break;
            default         : CUSTOM_FPRINTF(fout, "  unknown "); break;
        }
        
        if( port->range != NULL ) {
            CUSTOM_FPRINTF( fout, "[");
            strSize += 1;
            strSize += PrintRangeNumber( fout, port->range->upper->primary->value.number );
            // cout << port->range->upper->primary->value_type << endl;
            strSize += PrintRangeNumber( fout, port->range->lower->primary->value.number );
            CUSTOM_FPRINTF( fout, " ");
            strSize += 1;
        }

        int nameCnt = port->port_names->items;
        for(int j = 0; j < nameCnt; j++) {
            ast_identifier name = (ast_identifier)ast_list_get(port -> port_names, j);

            strSize += PrintIdentifier( fout, name ); 
            if( j != nameCnt-1) {
                CUSTOM_FPRINTF(fout, ", ");
            }

            if( strSize > VERILOG_LINE_MAX ) {
                CUSTOM_FPRINTF(fout, "\n       ");
                strSize = 0;
            }
        }
        CUSTOM_FPRINTF(fout, ";\n");
    }
    fflush(fout);
   
    // print wire info 
    strSize = 0;
    int wireCnt = module->net_declarations->items;
    if( wireCnt > 0 ) {
        CUSTOM_FPRINTF(fout, "  wire ");
    }
    for(int i = 0; i < wireCnt ; i++) {
        ast_net_declaration* net = (ast_net_declaration*) ast_list_get(module->net_declarations, i);
        CUSTOM_FPRINTF(fout, "%s", ast_identifier_tostring(net->identifier) );
        
        if( i != wireCnt-1 ) {
            CUSTOM_FPRINTF(fout, ", ");
        }
        strSize += strlen( ast_identifier_tostring(net->identifier) );
        if( strSize > VERILOG_LINE_MAX ) {
            CUSTOM_FPRINTF(fout, "\n       ");
            strSize = 0;
        }
    }
    if( wireCnt > 0 ) {
        CUSTOM_FPRINTF(fout, ";\n");
    }

    for(int i = 0; i < module -> module_instantiations -> items; i ++) {
        ast_module_instantiation * inst = 
                            (ast_module_instantiation*)ast_list_get(module->module_instantiations,i);

        // this is another module in other verilog.
        if( inst->resolved ) {
            ast_module_declaration* module = inst->declaration;
            CUSTOM_FPRINTF( fout, "  %s", ast_identifier_tostring(module->identifier));
//            cout << "instCount: " << inst->module_instances->items << endl;
//            cout << "paramCount: " << inst->module_parameters->items << endl;

            for(int j=0; j < inst->module_instances->items; j++) {
                ast_module_instance* curInst = (ast_module_instance*) 
                    ast_list_get( inst->module_instances, j);
                CUSTOM_FPRINTF(fout, " %s ( ", ast_identifier_tostring(curInst -> instance_identifier));

                int numConnection = curInst->port_connections->items;
//                cout << endl << "numConnection: " << numConnection << endl;
                for(int k=0; k<numConnection; k++) {
                    ast_port_connection* port = (ast_port_connection*) 
                        ast_list_get( curInst->port_connections, k );
                    CUSTOM_FPRINTF(fout, ".");
                    PrintIdentifier( fout, port->port_name );

                    CUSTOM_FPRINTF(fout, "(");
//                    PrintPrimary( fout, port->expression->primary );
                    PrintExpression( fout, port->expression );
                    CUSTOM_FPRINTF(fout, ")");
//                    cout << port -> expression -> primary->primary_type << endl;
//                    cout << port -> expression -> primary->value_type << endl;
//                    CUSTOM_FPRINTF(fout, "(%s)", ast_identifier_tostring(port->expression->primary->value.identifier));

//                    CUSTOM_FPRINTF(fout, ".%s(%s)", ast_identifier_tostring( port->port_name ),
//                            ast_identifier_tostring(port->expression->primary->value.identifier) ); 
                    if( k != numConnection-1 ) {
                        CUSTOM_FPRINTF(fout, ", ");
                    }
                }
                CUSTOM_FPRINTF(fout, " );\n");
            }
//            assert(0);
        }
        else {
            CUSTOM_FPRINTF( fout, "  %s", ast_identifier_tostring(inst->module_identifer) );

            for(int j=0; j < inst->module_instances->items; j++) {
                ast_module_instance* curInst = (ast_module_instance*) 
                    ast_list_get( inst->module_instances, j);
                CUSTOM_FPRINTF(fout, " %s ( ", ast_identifier_tostring(curInst -> instance_identifier));

                int numConnection = curInst->port_connections->items;
                for(int k=0; k<numConnection; k++) {
                    ast_port_connection* port = (ast_port_connection*) 
                        ast_list_get( curInst->port_connections, k);
                    // Error
                    CUSTOM_FPRINTF(fout, ".");
                    PrintIdentifier( fout, port->port_name );

                    CUSTOM_FPRINTF(fout, "(");
//                    PrintPrimary( fout, port->expression->primary );
                    PrintExpression( fout, port->expression );
                    CUSTOM_FPRINTF(fout, ")");

//                    CUSTOM_FPRINTF(fout, ".%s(%s)", ast_identifier_tostring( port->port_name ),
//                            ast_identifier_tostring(port->expression->primary->value.identifier) ); 

                    if( k != numConnection-1 ) {
                        CUSTOM_FPRINTF(fout, ", ");
                    }
                }
                CUSTOM_FPRINTF(fout, " );\n");
            }
        }
    }
    CUSTOM_FPRINTF(fout, "endmodule\n");
}

/*!
@brief Recursively decends the syntax tree, emiting the dot graph as it goes.
*/

// Top Level - start from root
void PrintVerilog( FILE* fout, verilog_source_tree * tree ) {

//    dot_node root = dot_new_node(graph);
//    dot_emit_node(graph,root,"Tree Root");
//    dot_node mod_hier = dot_new_node(graph);
//    dot_emit_edge(graph,root,mod_hier);
//    dot_emit_node(graph, mod_hier, "Module Hierarchy");
    
    for(int m = 0; m < tree -> modules -> items; m ++)
    {
        ast_module_declaration * module = (ast_module_declaration*)ast_list_get(tree->modules, m);
        PrintModule(
            fout,
            module
        );
    }
}

NAMESPACE_VERILOG_END
