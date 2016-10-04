var indexImage=1;
var c;
function changeImage(){

	//改变图片的透明度
	/*
	$(function(){
		$("#topImage").fadeTo(2000,0,function(){
			$("#topImage").fadeTo(0,1,function(){});
		});
	});
	*/
	e=document.getElementById("topImage");
	switch(indexImage){
		case 0:
		e.src="imgs/android.jpg";
		setImage(0);
		break;
		
		case 1:
		e.src="imgs/firewomen.jpg";
		setImage(1);
		break;
		
		case 2:
		e.src="imgs/run.jpg";
		setImage(2);
		break;
		
		case 3:
		e.src="imgs/love.jpg";
		setImage(3);
		break;
		
		default:
		e.src="imgs/firewomen.jpg";
		setImage(1);
		break;
		}
		indexImage+=1;
		indexImage%=4;
		//setTimeout("changeImage()",2000);
	}
function setOnLoad(){
	//加载初始状态
	setImage(0);
	
	//设置周期滚动图片
	setInterval("changeImage()",2000);
	
	//设置滚动
	setTimeout("setScroll()",10000);
	}

//设置滚动效果
function setScroll(){
	//setInterval("scrollBy(0,1)",10);
	//setTimeout("scrollTo(0,200)",200);
	}
function setImage(index){
	var a=new Array();
	a[0]=document.getElementById("tag0");
	a[1]=document.getElementById("tag1");
	a[2]=document.getElementById("tag2");
	a[3]=document.getElementById("tag3");
	for (var i=0; i<4; i++){
		if(i==index){
			a[i].src="imgs/press.png";
		}else{
			a[i].src="imgs/unpress.png";
		}
	}
}
function changeTag(index){
		indexImage=index;
		e=document.getElementById("topImage");
		switch(index){
		case 0:
		e.src="imgs/android.jpg";
		setImage(0);
		break;
		
		case 1:
		e.src="imgs/firewomen.jpg";
		setImage(1);
		break;
		
		case 2:
		e.src="imgs/run.jpg";
		setImage(2);
		break;
		
		case 3:
		e.src="imgs/love.jpg";
		setImage(3);
		break;
		
		default:
		e.src="imgs/firewomen.jpg";
		setImage(1);
		break;
		}
	}
function pressButton(){
	var num=document.getElementById("inputtext1");
	var password=document.getElementById("inputtext2");
	document.write(num.value+password.value);
	}

//那段视屏的播放
function playVideo(){
	var l=document.getElementById("logo");
	c=document.getElementById("titleBar");
	l.removeChild(c);
	l.innerHTML='<embed id="videos" src="movices/forvalues.mp4" height="210px" width="240px" autostart="true" />';
	}