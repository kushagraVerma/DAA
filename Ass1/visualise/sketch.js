let polys = [];

function addPolys(s){
    //expects "(x1,y1) (x2,y2)...
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

let bodyElt, canvElt, undoButt, resetButt, copyButt, addButt, addAr;
function setupButtons(){
    bodyElt = document.body;
    canvElt = document.getElementsByTagName("canvas");
    bodyElt.appendChild(document.createElement("br"));
    undoButt = document.createElement("button");
    resetButt = document.createElement("button");
    copyButt = document.createElement("button");
    addButt = document.createElement("button");
    addAr = document.createElement("textarea");
    addAr.cols = "20";
    undoButt.innerText = "UNDO";
    resetButt.innerText = "RESET";
    copyButt.innerText = "COPY POLY DATA";
    addButt.innerText = "ADD POLY";
    undoButt.onclick = ()=>{
        polys[polys.length-1]?.pts?.pop();
    };
    resetButt.onclick = ()=>{
        polys=[];
        addAr.value="";
    }
    copyButt.onclick = ()=>{
        let polyStr = polys[0].printPoly();
        navigator.clipboard.writeText(polyStr);
    };
    addButt.onclick = ()=>addPolys(addAr.value);
    bodyElt.appendChild(undoButt);
    bodyElt.appendChild(resetButt);
    bodyElt.appendChild(copyButt);
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

function draw() {
    background(0);
    push();
    stroke(255);
    strokeWeight(2);
    line(width/2,0,width/2,height);
    line(0,height/2,width,height/2);
    pop();
    for(let poly of polys){
        poly.drawPoly();
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
        let x = mouseX-width/2;
        let y = -(mouseY-height/2);
        polys[polys.length-1].addPt(x,y);
    }
}