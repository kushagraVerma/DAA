// let points = [];
// let lines = [];
// let lineColors = [];
let pts_line_arr = [];
let transform = {dx:0,dy:0,ds:1};
let gridSize = 50;


function takeInp(s){
    //expects "(x1,y1) (x2,y2)...""
    // for(let poly of polys){
    //     poly.stroke = color(0);
    // }
    let inps = s.split("\n");
    let lnsplit = inps.map(inp => {
        let tmp = inp.split(" ");
        tmp[0] = tmp[0].slice(0,tmp[0].length-1).split(")");
        tmp[0] = tmp[0].map(pt_ => {
            let tmp_ = pt_.slice(1).split(",");
            return {x:Number(tmp_[0]), y:Number(tmp_[1])};
        });
        tmp[1] = Number(tmp[1]);
        tmp[2] = Number(tmp[2]);
        return tmp;
    });
    // console.log(lnsplit);
    for(let ln of lnsplit){
        pts_line_arr.push(new ptsNline(...ln));
    }
    // for(let line of lines){
    // let pts_ = inps[0].split(" ");
    // points = pts_.map(pt => {
    //     let pt_ = pt.slice(1,pt.length-1).split(",");
    //     let x_ = Number(pt_[0]); 
    //     let y_ = Number(pt_[1]);
    //     if(isNaN(x_) || isNaN(y_)) return null;
    //     return {x: x_, y: -y_};
    // }).filter(x=>x!=null);
    // // points.push(new Poly(pts));
    // // }
    // let lns_ = inps[1].split(" ");
    // // console.log(lns_);
    // lines = lns_.map(ln => {
    //     let ln_ = ln.slice(1,ln.length-1).split(",");
    //     let a_ = Number(ln_[0]); 
    //     let b_ = Number(ln_[1]);
    //     if(isNaN(a_) || isNaN(b_)) return null;
    //     let y = (x)=>-(a_*x+b_);
    //     return {x1: -1e6, y1: y(-1e6), x2: 1e6, y2: y(1e6)};
    // }).filter(x=>x!=null);
    // lineColors = lines.map(x => color(random(100,255),random(100,255),random(100,255)));
}

function centerPoly(){
    let minX = Number.POSITIVE_INFINITY;
    let maxX = Number.NEGATIVE_INFINITY;
    let minY = Number.POSITIVE_INFINITY;
    let maxY = Number.NEGATIVE_INFINITY;
    let points = [];
    for(let pts_line of pts_line_arr){
        points = points.concat(pts_line.pts);
    }
    for(let pt of points){
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
    let f32 = x => Math.floor(x*32)/32
    let s_ = f32(0.9*Math.min(width/(maxX-minX),height/(maxY-minY)));
    if(s_ != Infinity){
        transform.ds = s_;
        transform.dx = -f32(s_*s_*((minX+maxX)/2));
        transform.dy = -f32(s_*s_*((minY+maxY)/2));
    }
}

let drawgrid = true;
let bodyElt, canvElt, resetButt, copyButt, gridButt, focusButt;
let inpButt, inpAr;
function setupButtons(){
    bodyElt = document.body;
    canvElt = document.getElementsByTagName("canvas");
    bodyElt.appendChild(document.createElement("br"));
    resetButt = document.createElement("button");
    copyButt = document.createElement("button");
    gridButt = document.createElement("button");
    focusButt = document.createElement("button");
    inpButt = document.createElement("button");
    inpAr = document.createElement("textarea");
    inpAr.cols = "20";
    resetButt.innerText = "RESET";
    copyButt.innerText = "COPY POINTS";
    gridButt.innerText = "GRID TOGGLE";
    focusButt.innerText = "FOCUS";
    inpButt.innerText = "TAKE INPUT";
    resetButt.onclick = ()=>{
        copyButt.disabled = true;
        focusButt.disabled = true;
        points=[];
        lines=[];
        inpAr.value="";
        transform = {dx:0,dy:0,ds:1};
        gridSize = 50;
    }
    copyButt.onclick = ()=>{
        // let polyStr = polys[0].printPoly();
        // navigator.clipboard.writeText(polyStr);
        let ptStr = `${points.length}\n`;
        for(let pt of points){
            ptStr += `${pt.x} ${-pt.y}\n`;
        }
        // console.log(ptStr);
        navigator.clipboard.writeText(ptStr);
    };
    gridButt.onclick = ()=>{
        drawgrid = !drawgrid;
    };
    focusButt.onclick = ()=>{
        centerPoly();
    };
    inpButt.onclick = ()=>{
        takeInp(inpAr.value);
        // copyButt.disabled = points.length==0;
        focusButt.disabled = false;
    };
    copyButt.disabled = true;
    focusButt.disabled = true;
    bodyElt.appendChild(resetButt);
    bodyElt.appendChild(copyButt);
    bodyElt.appendChild(gridButt);
    bodyElt.appendChild(focusButt);
    bodyElt.appendChild(document.createElement("br"));
    bodyElt.appendChild(inpButt);
    bodyElt.appendChild(document.createElement("br"));
    bodyElt.appendChild(inpAr);
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
    // const {dx,dy,ds} = transform;
    background(0);
    if(drawgrid){
        grid();
    }
    for(let pts_line of pts_line_arr){
        pts_line.drawLn(transform);
    }
    for(let pts_line of pts_line_arr){
        pts_line.drawPts(drawgrid,transform);
    }
    // for(let poly of polys){
    //     poly.drawPoly(drawgrid,transform);
    // }
    // push();
    // translate(width/2+dx/ds,height/2+dy/ds);
    // noStroke();
    // for(let pt of points){
    //     fill(255,0,0);
    //     ellipse(pt.x*ds,pt.y*ds,4,4);
    //     if(drawgrid){
    //         textSize(10);
    //         fill(255,255,0);
    //         text(`${trunc(pt.x)},${-trunc(pt.y)}`,pt.x*ds,pt.y*ds);
    //     }
    // }
    // for(let i = 0; i < lines.length; i++){
    //     ln = lines[i];
    //     col = lineColors[i];
    //     // stroke(color(random(100,255),random(100,255),random(100,255)));
    //     stroke(col);
    //     line(ln.x1,ln.y1,ln.x2,ln.y2);
    // }
    // pop();
    // noLoop();
}

function mouseOut(){
    return mouseX<0 || mouseX>width || mouseY<0 || mouseY>height;
}

function handleLeftClick(){
    if(mouseOut()){
        return;
    }
    // if(polys.length==1){
    // copyButt.disabled = false;
    // focusButt.disabled = false;
    // let x = (mouseX-width/2-transform.dx/transform.ds)/transform.ds;
    // let y = (mouseY-height/2-transform.dy/transform.ds)/transform.ds;
    // points.push({x: x, y: y});
    // }else{
    //     copyButt.disabled = true;
    //     focusButt.disabled = true;
    // }
}

function mouseClicked(){
    if(mouseOut()){
        return;
    }
    if(mouseButton === LEFT){
        handleLeftClick();
    }
}

function mouseDragged(){
    if(mouseOut()){
        return false;
    }
    if(mouseButton === CENTER){
        transform.dx += (mouseX-pmouseX)*transform.ds;
        transform.dy += (mouseY-pmouseY)*transform.ds;
    }
    return false;
}
function mouseWheel(event){
    if(mouseOut()){
        return;
    }
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
