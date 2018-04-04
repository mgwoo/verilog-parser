/*!
@file verilog-ast-walk.c
@brief Contains definitions of functions and structures used to walk the
verilog source tree.
*/

#define VERILOG_LINE_MAX 45
#define CUSTOM_FPRINTF(fmt, ...) {if(fmt) {fprintf(fmt, ##__VA_ARGS__); fflush(stdout);}}


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

static int strBuffer = 60;

void LinePrint( FILE* fout, size_t& strCnt, size_t count ) {
    strCnt += count; 
    if( strCnt > strBuffer ) {
        CUSTOM_FPRINTF( fout, "\n" );
        CUSTOM_FPRINTF( fout, "       " );
        strCnt = 7;
    }
}

void PrintIdentifier( FILE* fout, ast_identifier& identifier, size_t& strSize ) {
    RaiseErrorForNull( identifier );

    char* getIdentifier = ast_identifier_tostring( identifier );
    CUSTOM_FPRINTF(fout, "%s", getIdentifier);

    strSize += strlen(getIdentifier);
    free(getIdentifier);
}

void PrintRangeNumber( FILE* fout, ast_number* number, size_t& strSize ) {
    CUSTOM_FPRINTF(fout,  "%s", number->as_bits ); 
    strSize += 2;
}

// this Part 'Must be' updated later
void PrintNumber( FILE* fout, ast_number* number, size_t& strSize ) {
    RaiseErrorForNull( number );

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
            strSize += strlen(number->as_bits);
            break;
        // not used
        case REP_INTEGER: 
            CUSTOM_FPRINTF( fout, "%d", number->as_int );
            break;
        case REP_FLOAT: 
            CUSTOM_FPRINTF( fout, "%f", number->as_float );
            break;
    }
    strSize += 2;
}

void PrintExpression( FILE* fout, ast_expression* expression, size_t& strSize ) {
    if( !expression ) { return; }
    RaiseErrorForNull( expression );
    
    char * tr;
    char * lhs;
    char * rhs;
    char * pri;
    char * cond;
    char * mid;
    char * op;

    switch(expression -> type) {
        case PRIMARY_EXPRESSION:
        case MODULE_PATH_PRIMARY_EXPRESSION:
            PrintPrimary(fout, expression -> primary, strSize );
            // added by mgwoo
            if(expression->right) {
                rhs = ast_expression_tostring(expression->right);
                op = strdup("[");
                strcat(op, rhs);
                CUSTOM_FPRINTF(fout, "%s", op);
                free(rhs);
                strSize += strlen(op) + 1;
            }
            break;
        case STRING_EXPRESSION:
            tr = ast_strdup(expression -> string);
            strSize = strlen(tr);
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
}

void PrintConcatenation( FILE* fout, ast_concatenation* concatenation, size_t& strSize ) {
    RaiseErrorForNull( concatenation );
//    cout << "concatenation type: " << concatenation->type << endl;
    switch( concatenation->type ) {
        case CONCATENATION_EXPRESSION:
        case CONCATENATION_CONSTANT_EXPRESSION:
            PrintExpression( fout, concatenation->repeat, strSize );
            CUSTOM_FPRINTF( fout, ", " );
            strSize += 2;
            for(int i=0; i< concatenation->items->items; i++) {
                ast_expression* expr = (ast_expression*)ast_list_get( concatenation->items, i );
                PrintExpression( fout, expr, strSize );

                if( i != concatenation->items->items -1 ) {
                    CUSTOM_FPRINTF( fout, ", " );
                    LinePrint( fout, strSize, 2 );
                }
            }
            break;
        case CONCATENATION_NET:
        case CONCATENATION_VARIABLE:
        case CONCATENATION_MODULE_PATH:
            for( int i=0; i< concatenation->items->items; i++) {
                ast_identifier identifier = (ast_identifier)ast_list_get( concatenation->items, i );
                char* getIdentifier = ast_identifier_tostring(identifier); 
                fprintf( fout,"%s", getIdentifier );
                strSize += strlen( getIdentifier );
                free( getIdentifier );
            }
            break; 
    }
}

void PrintPrimary( FILE* fout, ast_primary* primary, size_t& strSize) {
    RaiseErrorForNull( primary );
    
    char* tmpchar = NULL;
//    cout << "primary type: " << primary->value_type << endl;
    switch( primary->value_type ) {
        case PRIMARY_NUMBER:        
            PrintNumber( fout, primary->value.number, strSize );
            break;
        case PRIMARY_IDENTIFIER:    
            PrintIdentifier( fout, primary->value.identifier, strSize );  
            break;
        case PRIMARY_CONCATENATION:
            CUSTOM_FPRINTF( fout, "{" ); 
            strSize += 1;
            PrintConcatenation( fout, primary->value.concatenation, strSize );
            CUSTOM_FPRINTF( fout, "}" );
            strSize += 1;
            break;
        case PRIMARY_FUNCTION_CALL:
            PrintIdentifier( fout, primary->value.function_call -> function, strSize );  
            break;
        case PRIMARY_MINMAX_EXP:
            tmpchar = ast_expression_tostring( primary->value.minmax );
            CUSTOM_FPRINTF( fout, "%s", tmpchar );
            strSize += strlen(tmpchar);
            free(tmpchar);
            break;
        case PRIMARY_MACRO_USAGE:
        default: break;
    }
}

void PrintLeftValue( FILE* fout, ast_lvalue* lvalue, size_t& strSize ) {
    RaiseErrorForNull( lvalue );
    switch( lvalue->type ) {
        case NET_IDENTIFIER:
        case VAR_IDENTIFIER:
        case GENVAR_IDENTIFIER:
            PrintIdentifier( fout, lvalue-> data.identifier, strSize ); break;
        case NET_CONCATENATION:
        case VAR_CONCATENATION:
            PrintConcatenation( fout, lvalue-> data.concatenation, strSize ); break;
    }
}

/*!
@brief Recursively walks the module declaration and instantiation hierarcy.
*/
void PrintModule ( FILE* fout, ast_module_declaration  * module ) {
    // print module title
    size_t strSize = 0;
    CUSTOM_FPRINTF(fout, "\nmodule %s ( ", module -> identifier->identifier);
    strSize += 10 + strlen(module->identifier->identifier); 
   
    int portCnt = module->module_ports->items; 

    for(int i = 0; i < portCnt; i++) {
        ast_port_declaration * port = (ast_port_declaration*)ast_list_get(module->module_ports,i);
        int nameCnt = port->port_names->items;
        for(int j = 0; j < nameCnt; j++) {
            ast_identifier name = (ast_identifier)ast_list_get(port -> port_names, j);
            PrintIdentifier( fout, name, strSize );
            if( !(i == portCnt-1 && j == nameCnt-1) ) {
                CUSTOM_FPRINTF(fout, ", ");
                LinePrint( fout, strSize, 2 );
            }
        }
    }
    CUSTOM_FPRINTF(fout, " );\n");
   
    // print port information 
    for(int i = 0; i < portCnt; i++) {
        strSize = 0;
        ast_port_declaration * port = (ast_port_declaration*)ast_list_get(module->module_ports,i);
        
        switch(port -> direction) {
            case PORT_INPUT : CUSTOM_FPRINTF(fout, "  input ");     strSize += 8;   break;    
            case PORT_OUTPUT: CUSTOM_FPRINTF(fout, "  output ");    strSize += 9;   break;
            case PORT_INOUT : CUSTOM_FPRINTF(fout, "  inout ");     strSize += 8;   break;  
            case PORT_NONE  : CUSTOM_FPRINTF(fout, "  unknown ");   strSize += 10;  break;     
            default         : CUSTOM_FPRINTF(fout, "  unknown ");   strSize += 10;  break;
        }
        
        if( port->range != NULL ) {
            CUSTOM_FPRINTF( fout, "[" );
            PrintRangeNumber( fout, port->range->upper->primary->value.number, strSize );
            PrintRangeNumber( fout, port->range->lower->primary->value.number, strSize );
            CUSTOM_FPRINTF( fout, " " );
            strSize += 2;
        }

        int nameCnt = port->port_names->items;
        for(int j = 0; j < nameCnt; j++) {
            ast_identifier name = (ast_identifier)ast_list_get(port -> port_names, j);

            PrintIdentifier( fout, name, strSize ); 
            if( j != nameCnt-1) {
                CUSTOM_FPRINTF(fout, ", ");
                LinePrint( fout, strSize, 2 );
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
        strSize += 7;
        for(int i = 0; i < wireCnt ; i++) {
            ast_net_declaration* net = (ast_net_declaration*) ast_list_get(module->net_declarations, i);
            PrintIdentifier( fout, net->identifier, strSize );
            
            if( i != wireCnt-1 ) {
                CUSTOM_FPRINTF(fout, ", ");
                LinePrint( fout, strSize, 2 );
            }
        }
        CUSTOM_FPRINTF(fout, ";\n");
    }

    // print assignment info
    strSize = 0;
    if( module->continuous_assignments ) {
        int assignCnt = module->continuous_assignments->items;
        for(int i=0; i<assignCnt; i++) {
            ast_continuous_assignment* assign = 
                (ast_continuous_assignment*) ast_list_get( module->continuous_assignments, i );
            CUSTOM_FPRINTF( fout, "  assign " );
            strSize = 9;
            for(int j=0; j<assign->assignments->items; j++ ) {
                ast_single_assignment* assignment = 
                    (ast_single_assignment*) ast_list_get( assign->assignments, j );
                PrintLeftValue( fout, assignment->lval, strSize );
                CUSTOM_FPRINTF( fout, " = " );
                strSize += 3;
                PrintExpression( fout, assignment->expression, strSize );
            }
            CUSTOM_FPRINTF( fout, ";\n");
            strSize = 0;
        }
    }

    strSize = 0;
    for(int i = 0; i < module -> module_instantiations -> items; i ++) {
        ast_module_instantiation * inst = 
                            (ast_module_instantiation*)ast_list_get(module->module_instantiations,i);

        // this is another module in other verilog.
        if( inst->resolved ) {
            ast_module_declaration* module = inst->declaration;

            CUSTOM_FPRINTF( fout, "  " );
            strSize += 2;
            PrintIdentifier( fout, module->identifier, strSize );

            for(int j=0; j < inst->module_instances->items; j++) {
                ast_module_instance* curInst = (ast_module_instance*) 
                    ast_list_get( inst->module_instances, j);
                
                CUSTOM_FPRINTF(fout, " ");
                PrintIdentifier( fout, curInst-> instance_identifier, strSize );

                CUSTOM_FPRINTF(fout, " ( ");
                strSize += 4;
                
                if( curInst->port_connections ) { 
                    int numConnection = curInst->port_connections->items;
                    for(int k=0; k<numConnection; k++) {
                        ast_port_connection* port = (ast_port_connection*) 
                            ast_list_get( curInst->port_connections, k );
                        CUSTOM_FPRINTF(fout, ".");
                        PrintIdentifier( fout, port->port_name, strSize );

                        CUSTOM_FPRINTF(fout, "(");
                        PrintExpression( fout, port->expression, strSize );
                        CUSTOM_FPRINTF(fout, ")");
                        strSize += 3;

                        if( k != numConnection-1 ) {
                            CUSTOM_FPRINTF(fout, ", ");
                            LinePrint( fout, strSize, 2 );
                        }
                    }
                }
                CUSTOM_FPRINTF(fout, " );\n");
                strSize = 0;
            }
        }
        else {
            CUSTOM_FPRINTF( fout, "  %s", ast_identifier_tostring(inst->module_identifer) );

            for(int j=0; j < inst->module_instances->items; j++) {
                ast_module_instance* curInst = (ast_module_instance*) 
                    ast_list_get( inst->module_instances, j);
                
                CUSTOM_FPRINTF(fout, " ");
                PrintIdentifier(fout, curInst -> instance_identifier, strSize);
                CUSTOM_FPRINTF(fout, " ( ");
                strSize += 4;
                
                if( curInst->port_connections ) {
                    int numConnection = curInst->port_connections->items;
                    for(int k=0; k<numConnection; k++) {
                        ast_port_connection* port = (ast_port_connection*) 
                            ast_list_get( curInst->port_connections, k);

                        CUSTOM_FPRINTF(fout, ".");
                        PrintIdentifier( fout, port->port_name, strSize );

                        CUSTOM_FPRINTF(fout, "(");
                        PrintExpression( fout, port->expression, strSize );
                        CUSTOM_FPRINTF(fout, ")");
                        strSize += 3;

                        if( k != numConnection-1 ) {
                            CUSTOM_FPRINTF(fout, ", ");
                            LinePrint( fout, strSize, 2 );
                        }
                    }
                }
                CUSTOM_FPRINTF(fout, " );\n");
                strSize = 0;
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
    for(int m = 0; m < tree -> modules -> items; m ++) {
        ast_module_declaration * module = (ast_module_declaration*)ast_list_get(tree->modules, m);
        PrintModule( fout, module );
    }
}

NAMESPACE_VERILOG_END
