from abc import ABC, abstractmethod

class Token:
    '''Tokenizer Class to properly store token values and their types
    '''
    def __init__(self, type_, value=None):
        self.type = type_
        self.value = value

    def __repr__(self):
        return f"Token({self.type}, {self.value})"

def tokenize(text):
    '''Takes a string and populates a list of token objects to make parsing easier. Used at highest level first before parsing

    Args:
        Text: a string representing a statement
    
    Returns:
        tokens: a list of parsed tokens with types and values
        '''

    tokens = []
    i = 0

    while i < len(text): 
        ch = text[i]

        if ch.isspace():
            i += 1

        elif ch.isdigit(): #consumes all digits and increases i
            j = i
            while j < len(text) and text[j].isdigit():
                j += 1
            tokens.append(Token("INT", int(text[i:j])))
            i = j

        elif ch.isalpha():
            j = i
            while j < len(text) and text[j].isalnum():
                j += 1
            word = text[i:j]
            if word == "print":
                tokens.append(Token("PRINT", word))
            elif word == "var":
                tokens.append(Token("VAR", word)) # declaration
            else:
                tokens.append(Token("IDENT", word)) # using variable for some purpose
            i = j

        elif ch in "+-*/=^();":
            tokens.append(Token(ch, ch))
            i += 1

        elif ch == '"':
            i += 1
            start_of_string = i

            while i < len(text) and text[i] != '"':
                i += 1

            if len(text) <= i: 
                raise Exception("Unterminated string")
            
            string_value = text[start_of_string:i]
            tokens.append(Token("STRING", string_value))
            i += 1 # skip end quote
            
        else:

            raise ValueError(f"Unknown character: {ch}")
        

    tokens.append(Token("EOF"))
    return tokens

class ASTNode(ABC): 
    @abstractmethod
    def __repr__(self):
        pass

class IntNode(ASTNode):
    def __init__(self, value): 
        self.value = value

    def __repr__(self):
        return f"IntNode: {self.value}"

class VarNode(ASTNode):
    def __init__(self, name): 
        self.name = name

    def __repr__(self):
        return f"VarNode: {self.name}"

class VarDeclNode(ASTNode):
    def __init__(self, name): 
        self.name = name

    def __repr__(self):
        return f"VarDeclNode: {self.name}"

class AssignNode(ASTNode):
    def __init__(self, name, expr):
        self.name = name
        self.expr = expr

    def __repr__(self):
        return f"AssignNode: {self.name} = {self.expr}"

class BinOpNode(ASTNode):
    def __init__(self, op, left, right):
        self.op = op
        self.left = left
        self.right = right

    def __repr__(self):
        return f"BinOpNode: {self.left}, {self.op}, {self.right}"

class PrintNode(ASTNode):
    def __init__(self, expr): 
        self.expr = expr

    def __repr__(self):
        return f"PrintNode: {self.expr}"

class StringNode(ASTNode):
    def __init__(self, string_value):
        self.string_value = string_value

    def __repr__(self):
        return f"StringNode: {self.string_value}"

class Parser:
    '''Top-level parser that takes in a list of tokens and traverses it with
    self.pos as position counter. Public functions will return nodes for evaluator to parse.

    Types of Grammar:
        stmt      
            := "var" IDENT ";" 
            | IDENT "=" expr ";" 
            | "print" "(" expr ")" ";"
        expr      
            := term (("+"|"-") term)*
        term      
            := factor (("*"|"/") factor)*
        factor    
            := INT | IDENT | "(" expr ")"
    '''
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0

    def peek(self):
        '''Looks at current token w/o consuming

        Returns:
            tokens at current index WITHOUT eating
        '''
        return self.tokens[self.pos]

    def eat(self, type_=None): 
        '''Consumes token and verifies if it is a certain type

        Args:
            type_: type of token, defaults to none

        Returns:
            token at current index WITH eating
        '''
        tok = self.tokens[self.pos]

        if type_ and tok.type != type_:
            raise ValueError(f"Expected {type_}, got {tok.type}")
        
        self.pos += 1
        return tok

    def parse(self): #parses a sequence of statements until EOF
        '''Entry point into parsing process
        
        Returns:
            Parsed statements into nodes
        '''
        stmts = []

        while self.peek().type != "EOF":
            stmts.append(self.parse_stmt())

        return stmts

    def parse_stmt(self): #parses a singular statement
        '''Parses a single statement, will pass onto helpers if encounters 
        different types of tokens
        
        Returns:
            Node type of parsed statement
        '''
        if self.peek().type == "VAR":
            self.eat("VAR")
            name = self.eat("IDENT").value
            self.eat(";")
            return VarDeclNode(name)

        elif self.peek().type == "IDENT":
            name = self.eat("IDENT").value
            self.eat("=")
            expr = self.parse_expr()
            self.eat(";")
            return AssignNode(name, expr)

        elif self.peek().type == "PRINT":
            self.eat("PRINT"); self.eat("(")
            expr = self.parse_expr()
            self.eat(")"); self.eat(";")
            return PrintNode(expr)

        else:
            raise ValueError("Invalid statement")

    def parse_expr(self):
        '''Parses higher levels of PEMDAS before doing lowest level (+ and -)

        Returns:
            BinOpNode for evaluator
        '''
        node = self.parse_term()

        while self.peek().type in ("+", "-"):
            op = self.eat().type
            right = self.parse_term()
            node = BinOpNode(op, node, right)
        return node

    def parse_term(self):
        '''Parses * and /, passes higher levels to parse_power()

        Returns:
            BinOpNode for evaluator
        '''
        node = self.parse_power()

        while self.peek().type in ("*", "/"):
            op = self.eat().type
            right = self.parse_power()
            node = BinOpNode(op, node, right)
        return node
    
    def parse_power(self):
        '''Parses power, passes higher levels to parse_factor()

        Returns:
            BinOpNode for evaluator
        '''

        node = self.parse_factor()

        while self.peek().type == "^":
            op = self.eat().type

            right = self.parse_power() # takes care of multiple power stacks 
            node = BinOpNode(op, node, right)

        return node

    def parse_factor(self):
        '''Parses lowest level value of nodes, then returns it upwards 
        for other functions to use

        Returns:
            Lowest level of node values
        '''
        tok = self.peek()

        if tok.type == "INT":
            self.eat("INT")
            return IntNode(tok.value)
        
        elif tok.type == "IDENT":
            name = self.eat("IDENT").value
            return VarNode(name)
        
        elif tok.type == "(":
            self.eat("(")
            node = self.parse_expr()
            self.eat(")")
            return node
        
        if tok.type == "STRING":
            self.eat("STRING")
            return StringNode(tok.value)
        
        else:
            raise ValueError(f"Unexpected token {tok}")

class Evaluator:
    '''Evaluates nodes while storing variables in current frame of reference 
    in environment dictionary
    '''

    def __init__(self):
        self.env = {}

    def eval(self, node):
        '''Evaluates already-parsed nodes, does the final arithmetic and 
        storing of variable values
        
        Returns:
            Final values of ASTNodes
        '''

        if isinstance(node, IntNode):
            return node.value
        
        elif isinstance(node, VarNode):
            if node.name not in self.env:
                raise NameError(f"Variable {node.name} not defined")
            
            return self.env[node.name]
        
        elif isinstance(node, VarDeclNode):
            if node.name in self.env:
                raise NameError(f"Variable {node.name} already declared")
            
            self.env[node.name] = None

        elif isinstance(node, AssignNode):
            if node.name not in self.env:
                raise NameError(f"Variable {node.name} not declared")
            
            self.env[node.name] = self.eval(node.expr)

        elif isinstance(node, BinOpNode):
            l = self.eval(node.left)
            r = self.eval(node.right)

            if node.op == "+": 
                return str(l) + str(r) if isinstance(l, str) or isinstance(r, str) else l + r
            
            if node.op == "-": 
                return l - r
            
            if node.op == "*": 
                return l * r
            
            if node.op == "/": 
                return l // r
            
            if node.op == "^": 
                return l ** r

        elif isinstance(node, PrintNode):
            val = self.eval(node.expr)
            print(val)

        elif isinstance(node, StringNode):
            return node.string_value

        else:
            raise ValueError("Unknown AST node")

# --------------------------
# Main driver
# --------------------------
def main():
    program = '''
    var x;
    var y;
    var z;
    x = 5 + 2 + 3;
    y = x * 13 + 12;
    z = x ^ 2;
    print(x);
    print(y + 1);
    print(z);
    print("bruh");
    print("bruh" + z);
    print("bruh" + "skisodu");
    '''
    tokens = tokenize(program)
    parser = Parser(tokens)
    stmts = parser.parse()
    evaluator = Evaluator()

    for stmt in stmts:
        evaluator.eval(stmt)

if __name__ == "__main__":
    main()
