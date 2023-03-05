let polys = [];
let transform = {dx:0,dy:0,ds:1};
let gridSize = 50;


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
            if(isNaN(x_) || isNaN(y_)) return null;
            return {x: x_, y: -y_};
        }).filter(x=>x!=null);
        polys.push(new Poly(pts));
    }
}

function centerPoly(){
    let minX = Number.POSITIVE_INFINITY;
    let maxX = Number.NEGATIVE_INFINITY;
    let minY = Number.POSITIVE_INFINITY;
    let maxY = Number.NEGATIVE_INFINITY;
    for(let poly of polys){
        for(let pt of poly.pts){
            // if(!pt.x || !pt.y) console.log(pt);
            if(pt.x < minX){
                minX = pt.x;
            }
            if(pt.x > maxX){
                maxX = pt.x;
            }
            if(pt.y < minY){
                minY = pt.y;
            }
            if(pt.y > maxY){
                maxY = pt.y;
            }
        }
    }
    let f32 = x => Math.floor(x*32)/32
    let s_ = f32(0.9*Math.min(width/(maxX-minX),height/(maxY-minY)));
    if(s_ != Infinity){
        transform.ds = s_;
        transform.dx = -f32(s_*s_*((minX+maxX)/2));
        transform.dy = -f32(s_*s_*((minY+maxY)/2));
    }
}

let drawgrid = true;
let bodyElt, canvElt, undoButt, resetButt, copyButt, gridButt, focusButt;
let addButt, addAr;
function setupButtons(){
    bodyElt = document.body;
    canvElt = document.getElementsByTagName("canvas");
    bodyElt.appendChild(document.createElement("br"));
    undoButt = document.createElement("button");
    resetButt = document.createElement("button");
    copyButt = document.createElement("button");
    gridButt = document.createElement("button");
    focusButt = document.createElement("button");
    addButt = document.createElement("button");
    addAr = document.createElement("textarea");
    addAr.cols = "20";
    undoButt.innerText = "UNDO";
    resetButt.innerText = "RESET";
    copyButt.innerText = "COPY POLY DATA";
    gridButt.innerText = "GRID TOGGLE";
    focusButt.innerText = "FOCUS";
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
        focusButt.disabled = true;
        polys=[];
        addAr.value="";
        transform = {dx:0,dy:0,ds:1};
        gridSize = 50;
    }
    copyButt.onclick = ()=>{
        let polyStr = polys[0].printPoly();
        navigator.clipboard.writeText(polyStr);
    };
    gridButt.onclick = ()=>{
        drawgrid = !drawgrid;
    };
    focusButt.onclick = ()=>{
        centerPoly();
    };
    addButt.onclick = ()=>{
        addPolys(addAr.value);
        undoButt.disabled = polys.length!=0;
        copyButt.disabled = polys.length!=0;
        focusButt.disabled = false;
    };
    undoButt.disabled = true;
    copyButt.disabled = true;
    focusButt.disabled = true;
    bodyElt.appendChild(undoButt);
    bodyElt.appendChild(resetButt);
    bodyElt.appendChild(copyButt);
    bodyElt.appendChild(gridButt);
    bodyElt.appendChild(focusButt);
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
    let dx = width/2+transform.dx/transform.ds;
    let dy = height/2+transform.dy/transform.ds;
    push();
    stroke(0,0,255);
    strokeWeight(3);
    line(dx,0,dx,height);
    line(0,dy,width,dy);
    strokeWeight(1);
    let gridSz = gridSize*transform.ds;
    for (let x = dx; x < width; x+=gridSz) {
        line(x,0,x,height);
    }
    for (let x = dx; x > 0; x-=gridSz) {
        line(x,0,x,height);
    }
    for (let y = dy; y < height; y+=gridSz) {
        line(0,y,width,y);
    }
    for (let y = dy; y > 0; y-=gridSz) {
        line(0,y,width,y);
    }
    pop();
    push();
    fill(0,255,255);
    textAlign(LEFT,TOP);
    text(`dx:${transform.dx} dy:${transform.dy} ds:${transform.ds}\nGrid size = ${gridSize}`,5,5);
    pop();
}

function draw() {
    background(0);
    if(drawgrid){
        grid();
    }
    for(let poly of polys){
        poly.drawPoly(drawgrid,transform);
    }
}

// function mouseOut(){
//     return mouseX<0 || mouseX>width || mouseY<0 || mouseY<height;
// }

function handleLeftClick(){
    if(mouseX<0 || mouseX>width || mouseY<0 || mouseY>width){
        return;
    }
    if(!polys.length){
        polys.push(new Poly());
    }
    if(polys.length==1){
        undoButt.disabled = false;
        copyButt.disabled = false;
        focusButt.disabled = false;
        let x = (mouseX-width/2-transform.dx/transform.ds)/transform.ds;
        let y = -(mouseY-height/2-transform.dy/transform.ds)/transform.ds;
        polys[polys.length-1].addPt(x,y);
    }else{
        undoButt.disabled = true;
        copyButt.disabled = true;
        focusButt.disabled = true;
    }
}

function mouseClicked(){
    // if(mouseOut()){
    //     return;
    // }
    if(mouseButton === LEFT){
        handleLeftClick();
    }
}

function mouseDragged(){
    // if(mouseOut()){
    //     return false;
    // }
    if(mouseButton === CENTER){
        transform.dx += (mouseX-pmouseX)*transform.ds;
        transform.dy += (mouseY-pmouseY)*transform.ds;
    }
    return false;
}
function mouseWheel(event){
    // if(mouseOut()){
    //     return;
    // }
    let d = event.delta/1000;
    if(keyIsDown(SHIFT)){
        d/=4;
    }
    if(keyIsDown(32)){ //SPACE
        if(gridSize - d*8 > 0){
            gridSize -= d*8;
        }
    }else{
        if(transform.ds - d > 0){
            transform.ds -= d;
        }
    }
}