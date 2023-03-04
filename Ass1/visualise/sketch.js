let polys = [];

function addPolys(s){
    //expects "(x1,y1) (x2,y2)...
    for(let poly of polys){
        poly.stroke = color(0);
    }
    let lines = s.split("\n");
    for(let line of lines){
        let pts_ = line.split(" ");
        let pts = pts_.map(pt => {
            let pt_ = pt.slice(1,pt.length-1).split(",");
            let x_ = Number(pt_[0]); 
            let y_ = Number(pt_[1]);
            return {x: x_, y: -y_};
        });
        polys.push(new Poly(pts));
    }
}

let drawgrid = true;
let bodyElt, canvElt, undoButt, resetButt, copyButt, gridButt, addButt, addAr;
function setupButtons(){
    bodyElt = document.body;
    canvElt = document.getElementsByTagName("canvas");
    bodyElt.appendChild(document.createElement("br"));
    undoButt = document.createElement("button");
    resetButt = document.createElement("button");
    copyButt = document.createElement("button");
    gridButt = document.createElement("button");
    addButt = document.createElement("button");
    addAr = document.createElement("textarea");
    addAr.cols = "20";
    undoButt.innerText = "UNDO";
    resetButt.innerText = "RESET";
    copyButt.innerText = "COPY POLY DATA";
    gridButt.innerText = "GRID TOGGLE";
    addButt.innerText = "ADD POLYS";
    undoButt.onclick = ()=>{
        if(polys[polys.length-1]?.pts.length==1){
            polys.pop();
            if(!polys.length){
                undoButt.disabled = true;
                copyButt.disabled = true;
            }
        }else{
            polys[polys.length-1]?.pts.pop();
        }
    };
    resetButt.onclick = ()=>{
        undoButt.disabled = true;
        copyButt.disabled = true;
        polys=[];
        addAr.value="";
    }
    copyButt.onclick = ()=>{
        let polyStr = polys[0].printPoly();
        navigator.clipboard.writeText(polyStr);
    };
    gridButt.onclick = ()=>{
        drawgrid = !drawgrid;
    };
    addButt.onclick = ()=>{
        undoButt.disabled = false;
        copyButt.disabled = false;
        addPolys(addAr.value);
    };
    undoButt.disabled = true;
    copyButt.disabled = true;
    bodyElt.appendChild(undoButt);
    bodyElt.appendChild(resetButt);
    bodyElt.appendChild(copyButt);
    bodyElt.appendChild(gridButt);
    bodyElt.appendChild(document.createElement("br"));
    bodyElt.appendChild(addButt);
    bodyElt.appendChild(document.createElement("br"));
    bodyElt.appendChild(addAr);
}

function setup() {
    createCanvas(600,600);
    background(0);
    textAlign(CENTER,CENTER);
    setupButtons();
}

function grid(){
    push();
    stroke(0,0,255);
    strokeWeight(3);
    line(width/2,0,width/2,height);
    line(0,height/2,width,height/2);
    strokeWeight(1);
    for (let x = 50; x < width; x+=50) {
        line(x,0,x,height);
    }
    for (let y = 50; y < height; y+=50) {
        line(0,y,width,y);
    }
    pop();
}

function draw() {
    background(0);
    if(drawgrid){
        grid();
    }
    for(let poly of polys){
        poly.drawPoly(drawgrid);
    }
}

function mouseClicked(){
    if(mouseX<0 || mouseX>width || mouseY<0 || mouseY>width){
        return;
    }
    if(!polys.length){
        polys.push(new Poly());
    }
    if(polys.length==1){
        undoButt.disabled = false;
        copyButt.disabled = false;
        let x = mouseX-width/2;
        let y = -(mouseY-height/2);
        polys[polys.length-1].addPt(x,y);
    }else{
        undoButt.disabled = true;
        copyButt.disabled = true;
    }
}