//============================
// ESTADOS ACADÉMICOS
//============================

const ESTADOS = [
    "REGULAR",
    "OBSERVADO",
    "RESERVADO",
    "EGRESADO"
];

//============================
// ESTUDIANTE
//============================

class Estudiante{

    constructor(codigo,nombre,escuela,ppa,estado){

        if(codigo<10000000 || codigo>99999999)
            throw "El código debe tener 8 dígitos.";

        if(ppa<0 || ppa>20)
            throw "El PPA debe estar entre 0 y 20.";

        this.codigo=codigo;
        this.nombre=nombre;
        this.escuela=escuela;
        this.ppa=ppa;
        this.estado=estado;

    }

}

//============================
// NODO BST
//============================

class Nodo{

    constructor(estudiante){

        this.estudiante=estudiante;

        this.izq=null;

        this.der=null;

    }

}

//============================
// ÁRBOL ACADÉMICO
//============================

class ArbolAcademico{

    constructor(){

        this.raiz=null;

    }

    //------------------------
    // INSERTAR
    //------------------------

    insertar(estudiante){

        this.raiz=this._insertar(this.raiz,estudiante);

    }

    _insertar(nodo,estudiante){

        if(nodo==null)
            return new Nodo(estudiante);

        if(estudiante.codigo<nodo.estudiante.codigo){

            nodo.izq=this._insertar(
                nodo.izq,
                estudiante
            );

        }

        else if(estudiante.codigo>nodo.estudiante.codigo){

            nodo.der=this._insertar(
                nodo.der,
                estudiante
            );

        }

        else{

            throw "Código ya registrado.";

        }

        return nodo;

    }

    //------------------------
    // BUSCAR
    //------------------------

    buscar(codigo){

        return this._buscar(
            this.raiz,
            codigo
        );

    }

    _buscar(nodo,codigo){

        if(nodo==null)
            return null;

        if(codigo==nodo.estudiante.codigo)
            return nodo.estudiante;

        if(codigo<nodo.estudiante.codigo){

            return this._buscar(
                nodo.izq,
                codigo
            );

        }

        return this._buscar(
            nodo.der,
            codigo
        );

    }

    //------------------------
    // MÍNIMO
    //------------------------

    minimo(nodo){

        let actual=nodo;

        while(actual && actual.izq){

            actual=actual.izq;

        }

        return actual;

    }

    //------------------------
    // ELIMINAR
    //------------------------

    eliminar(codigo){

        this.raiz=this._eliminar(
            this.raiz,
            codigo
        );

    }

    _eliminar(nodo,codigo){

        if(nodo==null){

            throw "No existe el estudiante.";

        }

        if(codigo<nodo.estudiante.codigo){

            nodo.izq=this._eliminar(
                nodo.izq,
                codigo
            );

        }

        else if(codigo>nodo.estudiante.codigo){

            nodo.der=this._eliminar(
                nodo.der,
                codigo
            );

        }

        else{

            if(nodo.izq==null)
                return nodo.der;

            if(nodo.der==null)
                return nodo.izq;

            let sucesor=this.minimo(
                nodo.der
            );

            nodo.estudiante=
                sucesor.estudiante;

            nodo.der=this._eliminar(
                nodo.der,
                sucesor.estudiante.codigo
            );

        }

        return nodo;

    }

    //------------------------
    // INORDER
    //------------------------

    inOrder(){

        let lista=[];

        this._inOrder(
            this.raiz,
            lista
        );

        return lista;

    }

    _inOrder(nodo,lista){

        if(nodo){

            this._inOrder(
                nodo.izq,
                lista
            );

            lista.push(
                nodo.estudiante
            );

            this._inOrder(
                nodo.der,
                lista
            );

        }

    }

    //------------------------
    // BFS
    //------------------------

    bfs(){

        let lista=[];

        let cola=[];

        if(this.raiz)
            cola.push(this.raiz);

        while(cola.length>0){

            let actual=cola.shift();

            lista.push(
                actual.estudiante
            );

            if(actual.izq)
                cola.push(
                    actual.izq
                );

            if(actual.der)
                cola.push(
                    actual.der
                );

        }

        return lista;

    }

}

//============================
// ÁRBOL GLOBAL
//============================

let arbol=new ArbolAcademico();

//======================================
// UTILIDADES
//======================================

function mostrarMensaje(texto,tipo){

    let div=document.getElementById("resultado");

    div.innerHTML=`<div class="${tipo}">
        ${texto}
    </div>`;

}

function crearTabla(lista){

    if(lista.length==0){

        mostrarMensaje(
            "No existen registros.",
            "info"
        );

        return;
    }

    let html="";

    html+="<table class='tabla'>";

    html+="<tr>";

    html+="<th>Código</th>";
    html+="<th>Nombre</th>";
    html+="<th>Escuela</th>";
    html+="<th>PPA</th>";
    html+="<th>Estado</th>";

    html+="</tr>";

    lista.forEach(e=>{

        html+="<tr>";

        html+=`<td>${e.codigo}</td>`;
        html+=`<td>${e.nombre}</td>`;
        html+=`<td>${e.escuela}</td>`;
        html+=`<td>${e.ppa}</td>`;
        html+=`<td>${e.estado}</td>`;

        html+="</tr>";

    });

    html+="</table>";

    document.getElementById(
        "resultado"
    ).innerHTML=html;

}

//======================================
// INSERTAR
//======================================

function insertarEstudiante(){

    try{

        let codigo=parseInt(
            document.getElementById(
                "codigo"
            ).value
        );

        let nombre=
        document.getElementById(
            "nombre"
        ).value;

        let escuela=
        document.getElementById(
            "escuela"
        ).value;

        let ppa=parseFloat(
            document.getElementById(
                "ppa"
            ).value
        );

        let estado=
        document.getElementById(
            "estado"
        ).value;

        let estudiante=
        new Estudiante(
            codigo,
            nombre,
            escuela,
            ppa,
            estado
        );

        arbol.insertar(
            estudiante
        );

        mostrarMensaje(
            "Estudiante agregado correctamente.",
            "exito"
        );

    }

    catch(error){

        mostrarMensaje(
            error,
            "error"
        );

    }

}

//======================================
// BUSCAR
//======================================

function buscarEstudiante(){

    let codigo=parseInt(

        document.getElementById(
            "buscarCodigo"
        ).value

    );

    let estudiante=
    arbol.buscar(
        codigo
    );

    if(estudiante==null){

        mostrarMensaje(
            "No encontrado.",
            "error"
        );

        return;

    }

    crearTabla([
        estudiante
    ]);

}

//======================================
// ELIMINAR
//======================================

function eliminarEstudiante(){

    try{

        let codigo=parseInt(

            document.getElementById(
                "eliminarCodigo"
            ).value

        );

        arbol.eliminar(
            codigo
        );

        mostrarMensaje(

            "Registro eliminado.",

            "exito"

        );

    }

    catch(error){

        mostrarMensaje(

            error,

            "error"

        );

    }

}

//======================================
// LISTAR INORDER
//======================================

function mostrarInOrder(){

    crearTabla(

        arbol.inOrder()

    );

}

//======================================
// LISTAR BFS
//======================================

function mostrarBFS(){

    crearTabla(

        arbol.bfs()

    );

}

//============================
// FILTRO POR PPA
//============================

ArbolAcademico.prototype.porPPA=function(min,max){

    let lista=this.inOrder();

    return lista.filter(e=>

        e.ppa>=min &&
        e.ppa<=max

    );

};

function filtrarPPA(){

    let min=parseFloat(
        document.getElementById(
            "ppaMin"
        ).value
    );

    let max=parseFloat(
        document.getElementById(
            "ppaMax"
        ).value
    );

    crearTabla(

        arbol.porPPA(
            min,
            max
        )

    );

}