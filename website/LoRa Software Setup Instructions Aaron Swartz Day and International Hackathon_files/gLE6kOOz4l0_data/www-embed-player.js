(function(){/*

 Copyright The Closure Library Authors.
 SPDX-License-Identifier: Apache-2.0
*/
var n;function aa(a){var b=0;return function(){return b<a.length?{done:!1,value:a[b++]}:{done:!0}}}
var ba="function"==typeof Object.defineProperties?Object.defineProperty:function(a,b,c){if(a==Array.prototype||a==Object.prototype)return a;a[b]=c.value;return a};
function ca(a){a=["object"==typeof globalThis&&globalThis,a,"object"==typeof window&&window,"object"==typeof self&&self,"object"==typeof global&&global];for(var b=0;b<a.length;++b){var c=a[b];if(c&&c.Math==Math)return c}throw Error("Cannot find global object");}
var da=ca(this);function q(a,b){if(b)a:{for(var c=da,d=a.split("."),e=0;e<d.length-1;e++){var f=d[e];if(!(f in c))break a;c=c[f]}d=d[d.length-1];e=c[d];f=b(e);f!=e&&null!=f&&ba(c,d,{configurable:!0,writable:!0,value:f})}}
q("Symbol",function(a){function b(e){if(this instanceof b)throw new TypeError("Symbol is not a constructor");return new c("jscomp_symbol_"+(e||"")+"_"+d++,e)}
function c(e,f){this.f=e;ba(this,"description",{configurable:!0,writable:!0,value:f})}
if(a)return a;c.prototype.toString=function(){return this.f};
var d=0;return b});
q("Symbol.iterator",function(a){if(a)return a;a=Symbol("Symbol.iterator");for(var b="Array Int8Array Uint8Array Uint8ClampedArray Int16Array Uint16Array Int32Array Uint32Array Float32Array Float64Array".split(" "),c=0;c<b.length;c++){var d=da[b[c]];"function"===typeof d&&"function"!=typeof d.prototype[a]&&ba(d.prototype,a,{configurable:!0,writable:!0,value:function(){return ea(aa(this))}})}return a});
function ea(a){a={next:a};a[Symbol.iterator]=function(){return this};
return a}
function t(a){var b="undefined"!=typeof Symbol&&Symbol.iterator&&a[Symbol.iterator];return b?b.call(a):{next:aa(a)}}
function fa(a){for(var b,c=[];!(b=a.next()).done;)c.push(b.value);return c}
var ha="function"==typeof Object.create?Object.create:function(a){function b(){}
b.prototype=a;return new b},ia=function(){function a(){function c(){}
new c;Reflect.construct(c,[],function(){});
return new c instanceof c}
if("undefined"!=typeof Reflect&&Reflect.construct){if(a())return Reflect.construct;var b=Reflect.construct;return function(c,d,e){c=b(c,d);e&&Reflect.setPrototypeOf(c,e.prototype);return c}}return function(c,d,e){void 0===e&&(e=c);
e=ha(e.prototype||Object.prototype);return Function.prototype.apply.call(c,e,d)||e}}(),ja;
if("function"==typeof Object.setPrototypeOf)ja=Object.setPrototypeOf;else{var ka;a:{var la={a:!0},ma={};try{ma.__proto__=la;ka=ma.a;break a}catch(a){}ka=!1}ja=ka?function(a,b){a.__proto__=b;if(a.__proto__!==b)throw new TypeError(a+" is not extensible");return a}:null}var na=ja;
function v(a,b){a.prototype=ha(b.prototype);a.prototype.constructor=a;if(na)na(a,b);else for(var c in b)if("prototype"!=c)if(Object.defineProperties){var d=Object.getOwnPropertyDescriptor(b,c);d&&Object.defineProperty(a,c,d)}else a[c]=b[c];a.I=b.prototype}
function oa(){this.l=!1;this.h=null;this.A=void 0;this.g=1;this.i=this.j=0;this.u=this.f=null}
function pa(a){if(a.l)throw new TypeError("Generator is already running");a.l=!0}
oa.prototype.m=function(a){this.A=a};
function qa(a,b){a.f={Ia:b,ua:!0};a.g=a.j||a.i}
oa.prototype["return"]=function(a){this.f={"return":a};this.g=this.i};
function ra(a,b){a.g=5;return{value:b}}
oa.prototype.oa=function(a){this.g=a};
function sa(a){a.j=2;a.i=3}
function ta(a){a.j=0;a.f=null}
function ua(a){a.u=[a.f];a.j=0;a.i=0}
function va(a){var b=a.u.splice(0)[0];(b=a.f=a.f||b)?b.ua?a.g=a.j||a.i:void 0!=b.oa&&a.i<b.oa?(a.g=b.oa,a.f=null):a.g=a.i:a.g=4}
function wa(a){this.f=new oa;this.g=a}
function xa(a,b){pa(a.f);var c=a.f.h;if(c)return ya(a,"return"in c?c["return"]:function(d){return{value:d,done:!0}},b,a.f["return"]);
a.f["return"](b);return za(a)}
function ya(a,b,c,d){try{var e=b.call(a.f.h,c);if(!(e instanceof Object))throw new TypeError("Iterator result "+e+" is not an object");if(!e.done)return a.f.l=!1,e;var f=e.value}catch(g){return a.f.h=null,qa(a.f,g),za(a)}a.f.h=null;d.call(a.f,f);return za(a)}
function za(a){for(;a.f.g;)try{var b=a.g(a.f);if(b)return a.f.l=!1,{value:b.value,done:!1}}catch(c){a.f.A=void 0,qa(a.f,c)}a.f.l=!1;if(a.f.f){b=a.f.f;a.f.f=null;if(b.ua)throw b.Ia;return{value:b["return"],done:!0}}return{value:void 0,done:!0}}
function Aa(a){this.next=function(b){pa(a.f);a.f.h?b=ya(a,a.f.h.next,b,a.f.m):(a.f.m(b),b=za(a));return b};
this["throw"]=function(b){pa(a.f);a.f.h?b=ya(a,a.f.h["throw"],b,a.f.m):(qa(a.f,b),b=za(a));return b};
this["return"]=function(b){return xa(a,b)};
this[Symbol.iterator]=function(){return this}}
function Ba(a,b){var c=new Aa(new wa(b));na&&a.prototype&&na(c,a.prototype);return c}
q("Reflect",function(a){return a?a:{}});
q("Reflect.construct",function(){return ia});
q("Reflect.setPrototypeOf",function(a){return a?a:na?function(b,c){try{return na(b,c),!0}catch(d){return!1}}:null});
function Ca(a,b,c){if(null==a)throw new TypeError("The 'this' value for String.prototype."+c+" must not be null or undefined");if(b instanceof RegExp)throw new TypeError("First argument to String.prototype."+c+" must not be a regular expression");return a+""}
q("String.prototype.endsWith",function(a){return a?a:function(b,c){var d=Ca(this,b,"endsWith");b+="";void 0===c&&(c=d.length);for(var e=Math.max(0,Math.min(c|0,d.length)),f=b.length;0<f&&0<e;)if(d[--e]!=b[--f])return!1;return 0>=f}});
q("String.prototype.startsWith",function(a){return a?a:function(b,c){var d=Ca(this,b,"startsWith");b+="";for(var e=d.length,f=b.length,g=Math.max(0,Math.min(c|0,d.length)),h=0;h<f&&g<e;)if(d[g++]!=b[h++])return!1;return h>=f}});
q("Object.setPrototypeOf",function(a){return a||na});
function Da(a,b){return Object.prototype.hasOwnProperty.call(a,b)}
var Ea="function"==typeof Object.assign?Object.assign:function(a,b){for(var c=1;c<arguments.length;c++){var d=arguments[c];if(d)for(var e in d)Da(d,e)&&(a[e]=d[e])}return a};
q("Object.assign",function(a){return a||Ea});
q("Promise",function(a){function b(g){this.f=0;this.h=void 0;this.g=[];this.m=!1;var h=this.i();try{g(h.resolve,h.reject)}catch(k){h.reject(k)}}
function c(){this.f=null}
function d(g){return g instanceof b?g:new b(function(h){h(g)})}
if(a)return a;c.prototype.g=function(g){if(null==this.f){this.f=[];var h=this;this.h(function(){h.j()})}this.f.push(g)};
var e=da.setTimeout;c.prototype.h=function(g){e(g,0)};
c.prototype.j=function(){for(;this.f&&this.f.length;){var g=this.f;this.f=[];for(var h=0;h<g.length;++h){var k=g[h];g[h]=null;try{k()}catch(l){this.i(l)}}}this.f=null};
c.prototype.i=function(g){this.h(function(){throw g;})};
b.prototype.i=function(){function g(l){return function(m){k||(k=!0,l.call(h,m))}}
var h=this,k=!1;return{resolve:g(this.G),reject:g(this.j)}};
b.prototype.G=function(g){if(g===this)this.j(new TypeError("A Promise cannot resolve to itself"));else if(g instanceof b)this.M(g);else{a:switch(typeof g){case "object":var h=null!=g;break a;case "function":h=!0;break a;default:h=!1}h?this.F(g):this.l(g)}};
b.prototype.F=function(g){var h=void 0;try{h=g.then}catch(k){this.j(k);return}"function"==typeof h?this.N(h,g):this.l(g)};
b.prototype.j=function(g){this.A(2,g)};
b.prototype.l=function(g){this.A(1,g)};
b.prototype.A=function(g,h){if(0!=this.f)throw Error("Cannot settle("+g+", "+h+"): Promise already settled in state"+this.f);this.f=g;this.h=h;2===this.f&&this.H();this.u()};
b.prototype.H=function(){var g=this;e(function(){if(g.C()){var h=da.console;"undefined"!==typeof h&&h.error(g.h)}},1)};
b.prototype.C=function(){if(this.m)return!1;var g=da.CustomEvent,h=da.Event,k=da.dispatchEvent;if("undefined"===typeof k)return!0;"function"===typeof g?g=new g("unhandledrejection",{cancelable:!0}):"function"===typeof h?g=new h("unhandledrejection",{cancelable:!0}):(g=da.document.createEvent("CustomEvent"),g.initCustomEvent("unhandledrejection",!1,!0,g));g.promise=this;g.reason=this.h;return k(g)};
b.prototype.u=function(){if(null!=this.g){for(var g=0;g<this.g.length;++g)f.g(this.g[g]);this.g=null}};
var f=new c;b.prototype.M=function(g){var h=this.i();g.ea(h.resolve,h.reject)};
b.prototype.N=function(g,h){var k=this.i();try{g.call(h,k.resolve,k.reject)}catch(l){k.reject(l)}};
b.prototype.then=function(g,h){function k(u,r){return"function"==typeof u?function(A){try{l(u(A))}catch(D){m(D)}}:r}
var l,m,p=new b(function(u,r){l=u;m=r});
this.ea(k(g,l),k(h,m));return p};
b.prototype["catch"]=function(g){return this.then(void 0,g)};
b.prototype.ea=function(g,h){function k(){switch(l.f){case 1:g(l.h);break;case 2:h(l.h);break;default:throw Error("Unexpected state: "+l.f);}}
var l=this;null==this.g?f.g(k):this.g.push(k);this.m=!0};
b.resolve=d;b.reject=function(g){return new b(function(h,k){k(g)})};
b.race=function(g){return new b(function(h,k){for(var l=t(g),m=l.next();!m.done;m=l.next())d(m.value).ea(h,k)})};
b.all=function(g){var h=t(g),k=h.next();return k.done?d([]):new b(function(l,m){function p(A){return function(D){u[A]=D;r--;0==r&&l(u)}}
var u=[],r=0;do u.push(void 0),r++,d(k.value).ea(p(u.length-1),m),k=h.next();while(!k.done)})};
return b});
function Fa(a,b){a instanceof String&&(a+="");var c=0,d=!1,e={next:function(){if(!d&&c<a.length){var f=c++;return{value:b(f,a[f]),done:!1}}d=!0;return{done:!0,value:void 0}}};
e[Symbol.iterator]=function(){return e};
return e}
q("Array.prototype.entries",function(a){return a?a:function(){return Fa(this,function(b,c){return[b,c]})}});
q("Array.prototype.keys",function(a){return a?a:function(){return Fa(this,function(b){return b})}});
q("Array.prototype.values",function(a){return a?a:function(){return Fa(this,function(b,c){return c})}});
q("Object.is",function(a){return a?a:function(b,c){return b===c?0!==b||1/b===1/c:b!==b&&c!==c}});
q("Array.prototype.includes",function(a){return a?a:function(b,c){var d=this;d instanceof String&&(d=String(d));var e=d.length,f=c||0;for(0>f&&(f=Math.max(f+e,0));f<e;f++){var g=d[f];if(g===b||Object.is(g,b))return!0}return!1}});
q("String.prototype.includes",function(a){return a?a:function(b,c){return-1!==Ca(this,b,"includes").indexOf(b,c||0)}});
q("Object.entries",function(a){return a?a:function(b){var c=[],d;for(d in b)Da(b,d)&&c.push([d,b[d]]);return c}});
q("WeakMap",function(a){function b(k){this.f=(h+=Math.random()+1).toString();if(k){k=t(k);for(var l;!(l=k.next()).done;)l=l.value,this.set(l[0],l[1])}}
function c(){}
function d(k){var l=typeof k;return"object"===l&&null!==k||"function"===l}
function e(k){if(!Da(k,g)){var l=new c;ba(k,g,{value:l})}}
function f(k){var l=Object[k];l&&(Object[k]=function(m){if(m instanceof c)return m;Object.isExtensible(m)&&e(m);return l(m)})}
if(function(){if(!a||!Object.seal)return!1;try{var k=Object.seal({}),l=Object.seal({}),m=new a([[k,2],[l,3]]);if(2!=m.get(k)||3!=m.get(l))return!1;m["delete"](k);m.set(l,4);return!m.has(k)&&4==m.get(l)}catch(p){return!1}}())return a;
var g="$jscomp_hidden_"+Math.random();f("freeze");f("preventExtensions");f("seal");var h=0;b.prototype.set=function(k,l){if(!d(k))throw Error("Invalid WeakMap key");e(k);if(!Da(k,g))throw Error("WeakMap key fail: "+k);k[g][this.f]=l;return this};
b.prototype.get=function(k){return d(k)&&Da(k,g)?k[g][this.f]:void 0};
b.prototype.has=function(k){return d(k)&&Da(k,g)&&Da(k[g],this.f)};
b.prototype["delete"]=function(k){return d(k)&&Da(k,g)&&Da(k[g],this.f)?delete k[g][this.f]:!1};
return b});
q("Map",function(a){function b(){var h={};return h.previous=h.next=h.head=h}
function c(h,k){var l=h.f;return ea(function(){if(l){for(;l.head!=h.f;)l=l.previous;for(;l.next!=l.head;)return l=l.next,{done:!1,value:k(l)};l=null}return{done:!0,value:void 0}})}
function d(h,k){var l=k&&typeof k;"object"==l||"function"==l?f.has(k)?l=f.get(k):(l=""+ ++g,f.set(k,l)):l="p_"+k;var m=h.g[l];if(m&&Da(h.g,l))for(var p=0;p<m.length;p++){var u=m[p];if(k!==k&&u.key!==u.key||k===u.key)return{id:l,list:m,index:p,B:u}}return{id:l,list:m,index:-1,B:void 0}}
function e(h){this.g={};this.f=b();this.size=0;if(h){h=t(h);for(var k;!(k=h.next()).done;)k=k.value,this.set(k[0],k[1])}}
if(function(){if(!a||"function"!=typeof a||!a.prototype.entries||"function"!=typeof Object.seal)return!1;try{var h=Object.seal({x:4}),k=new a(t([[h,"s"]]));if("s"!=k.get(h)||1!=k.size||k.get({x:4})||k.set({x:4},"t")!=k||2!=k.size)return!1;var l=k.entries(),m=l.next();if(m.done||m.value[0]!=h||"s"!=m.value[1])return!1;m=l.next();return m.done||4!=m.value[0].x||"t"!=m.value[1]||!l.next().done?!1:!0}catch(p){return!1}}())return a;
var f=new WeakMap;e.prototype.set=function(h,k){h=0===h?0:h;var l=d(this,h);l.list||(l.list=this.g[l.id]=[]);l.B?l.B.value=k:(l.B={next:this.f,previous:this.f.previous,head:this.f,key:h,value:k},l.list.push(l.B),this.f.previous.next=l.B,this.f.previous=l.B,this.size++);return this};
e.prototype["delete"]=function(h){h=d(this,h);return h.B&&h.list?(h.list.splice(h.index,1),h.list.length||delete this.g[h.id],h.B.previous.next=h.B.next,h.B.next.previous=h.B.previous,h.B.head=null,this.size--,!0):!1};
e.prototype.clear=function(){this.g={};this.f=this.f.previous=b();this.size=0};
e.prototype.has=function(h){return!!d(this,h).B};
e.prototype.get=function(h){return(h=d(this,h).B)&&h.value};
e.prototype.entries=function(){return c(this,function(h){return[h.key,h.value]})};
e.prototype.keys=function(){return c(this,function(h){return h.key})};
e.prototype.values=function(){return c(this,function(h){return h.value})};
e.prototype.forEach=function(h,k){for(var l=this.entries(),m;!(m=l.next()).done;)m=m.value,h.call(k,m[1],m[0],this)};
e.prototype[Symbol.iterator]=e.prototype.entries;var g=0;return e});
q("Set",function(a){function b(c){this.f=new Map;if(c){c=t(c);for(var d;!(d=c.next()).done;)this.add(d.value)}this.size=this.f.size}
if(function(){if(!a||"function"!=typeof a||!a.prototype.entries||"function"!=typeof Object.seal)return!1;try{var c=Object.seal({x:4}),d=new a(t([c]));if(!d.has(c)||1!=d.size||d.add(c)!=d||1!=d.size||d.add({x:4})!=d||2!=d.size)return!1;var e=d.entries(),f=e.next();if(f.done||f.value[0]!=c||f.value[1]!=c)return!1;f=e.next();return f.done||f.value[0]==c||4!=f.value[0].x||f.value[1]!=f.value[0]?!1:e.next().done}catch(g){return!1}}())return a;
b.prototype.add=function(c){c=0===c?0:c;this.f.set(c,c);this.size=this.f.size;return this};
b.prototype["delete"]=function(c){c=this.f["delete"](c);this.size=this.f.size;return c};
b.prototype.clear=function(){this.f.clear();this.size=0};
b.prototype.has=function(c){return this.f.has(c)};
b.prototype.entries=function(){return this.f.entries()};
b.prototype.values=function(){return this.f.values()};
b.prototype.keys=b.prototype.values;b.prototype[Symbol.iterator]=b.prototype.values;b.prototype.forEach=function(c,d){var e=this;this.f.forEach(function(f){return c.call(d,f,f,e)})};
return b});
var w=this||self;function x(a,b,c){a=a.split(".");c=c||w;a[0]in c||"undefined"==typeof c.execScript||c.execScript("var "+a[0]);for(var d;a.length&&(d=a.shift());)a.length||void 0===b?c[d]&&c[d]!==Object.prototype[d]?c=c[d]:c=c[d]={}:c[d]=b}
function Ga(a){if(a&&a!=w)return Ha(a.document);null===Ia&&(Ia=Ha(w.document));return Ia}
var Ja=/^[\w+/_-]+[=]{0,2}$/,Ia=null;function Ha(a){return(a=a.querySelector&&a.querySelector("script[nonce]"))&&(a=a.nonce||a.getAttribute("nonce"))&&Ja.test(a)?a:""}
function y(a,b){for(var c=a.split("."),d=b||w,e=0;e<c.length;e++)if(d=d[c[e]],null==d)return null;return d}
function z(){}
function Ka(a){a.na=void 0;a.getInstance=function(){return a.na?a.na:a.na=new a}}
function La(a){var b=typeof a;return"object"!=b?b:a?Array.isArray(a)?"array":b:"null"}
function Ma(a){var b=La(a);return"array"==b||"object"==b&&"number"==typeof a.length}
function B(a){var b=typeof a;return"object"==b&&null!=a||"function"==b}
function Na(a){return Object.prototype.hasOwnProperty.call(a,Oa)&&a[Oa]||(a[Oa]=++Pa)}
var Oa="closure_uid_"+(1E9*Math.random()>>>0),Pa=0;function Qa(a,b,c){return a.call.apply(a.bind,arguments)}
function Ra(a,b,c){if(!a)throw Error();if(2<arguments.length){var d=Array.prototype.slice.call(arguments,2);return function(){var e=Array.prototype.slice.call(arguments);Array.prototype.unshift.apply(e,d);return a.apply(b,e)}}return function(){return a.apply(b,arguments)}}
function C(a,b,c){Function.prototype.bind&&-1!=Function.prototype.bind.toString().indexOf("native code")?C=Qa:C=Ra;return C.apply(null,arguments)}
function Sa(a,b){var c=Array.prototype.slice.call(arguments,1);return function(){var d=c.slice();d.push.apply(d,arguments);return a.apply(this,d)}}
function E(){return Date.now()}
function Ua(a,b){x(a,b,void 0)}
function F(a,b){function c(){}
c.prototype=b.prototype;a.I=b.prototype;a.prototype=new c;a.prototype.constructor=a;a.zb=function(d,e,f){for(var g=Array(arguments.length-2),h=2;h<arguments.length;h++)g[h-2]=arguments[h];return b.prototype[e].apply(d,g)}}
function Va(a){return a}
;function G(a){if(Error.captureStackTrace)Error.captureStackTrace(this,G);else{var b=Error().stack;b&&(this.stack=b)}a&&(this.message=String(a))}
F(G,Error);G.prototype.name="CustomError";function Wa(a){a=a.url;var b=/[?&]dsh=1(&|$)/.test(a);this.h=!b&&/[?&]ae=1(&|$)/.test(a);this.i=!b&&/[?&]ae=2(&|$)/.test(a);if((this.f=/[?&]adurl=([^&]*)/.exec(a))&&this.f[1]){try{var c=decodeURIComponent(this.f[1])}catch(d){c=null}this.g=c}}
;function Xa(a){var b=!1,c;return function(){b||(c=a(),b=!0);return c}}
;var Ya=Array.prototype.indexOf?function(a,b){return Array.prototype.indexOf.call(a,b,void 0)}:function(a,b){if("string"===typeof a)return"string"!==typeof b||1!=b.length?-1:a.indexOf(b,0);
for(var c=0;c<a.length;c++)if(c in a&&a[c]===b)return c;return-1},H=Array.prototype.forEach?function(a,b,c){Array.prototype.forEach.call(a,b,c)}:function(a,b,c){for(var d=a.length,e="string"===typeof a?a.split(""):a,f=0;f<d;f++)f in e&&b.call(c,e[f],f,a)},Za=Array.prototype.filter?function(a,b){return Array.prototype.filter.call(a,b,void 0)}:function(a,b){for(var c=a.length,d=[],e=0,f="string"===typeof a?a.split(""):a,g=0;g<c;g++)if(g in f){var h=f[g];
b.call(void 0,h,g,a)&&(d[e++]=h)}return d},$a=Array.prototype.map?function(a,b){return Array.prototype.map.call(a,b,void 0)}:function(a,b){for(var c=a.length,d=Array(c),e="string"===typeof a?a.split(""):a,f=0;f<c;f++)f in e&&(d[f]=b.call(void 0,e[f],f,a));
return d},ab=Array.prototype.reduce?function(a,b,c){return Array.prototype.reduce.call(a,b,c)}:function(a,b,c){var d=c;
H(a,function(e,f){d=b.call(void 0,d,e,f,a)});
return d};
function bb(a,b){a:{var c=a.length;for(var d="string"===typeof a?a.split(""):a,e=0;e<c;e++)if(e in d&&b.call(void 0,d[e],e,a)){c=e;break a}c=-1}return 0>c?null:"string"===typeof a?a.charAt(c):a[c]}
function cb(a,b){var c=Ya(a,b),d;(d=0<=c)&&Array.prototype.splice.call(a,c,1);return d}
function db(a){var b=a.length;if(0<b){for(var c=Array(b),d=0;d<b;d++)c[d]=a[d];return c}return[]}
function eb(a,b){for(var c=1;c<arguments.length;c++){var d=arguments[c];if(Ma(d)){var e=a.length||0,f=d.length||0;a.length=e+f;for(var g=0;g<f;g++)a[e+g]=d[g]}else a.push(d)}}
;function fb(a,b){for(var c in a)b.call(void 0,a[c],c,a)}
function gb(a,b){var c=Ma(b),d=c?b:arguments;for(c=c?0:1;c<d.length;c++){if(null==a)return;a=a[d[c]]}return a}
function hb(a){var b=ib,c;for(c in b)if(a.call(void 0,b[c],c,b))return c}
function jb(a){for(var b in a)return!1;return!0}
function kb(a,b){if(null!==a&&b in a)throw Error('The object already contains the key "'+b+'"');a[b]=!0}
function lb(a,b){for(var c in a)if(!(c in b)||a[c]!==b[c])return!1;for(var d in b)if(!(d in a))return!1;return!0}
function mb(a){var b={},c;for(c in a)b[c]=a[c];return b}
function nb(a){if(!a||"object"!==typeof a)return a;if("function"===typeof a.clone)return a.clone();var b=Array.isArray(a)?[]:"function"!==typeof ArrayBuffer||"function"!==typeof ArrayBuffer.isView||!ArrayBuffer.isView(a)||a instanceof DataView?{}:new a.constructor(a.length),c;for(c in a)b[c]=nb(a[c]);return b}
var ob="constructor hasOwnProperty isPrototypeOf propertyIsEnumerable toLocaleString toString valueOf".split(" ");function pb(a,b){for(var c,d,e=1;e<arguments.length;e++){d=arguments[e];for(c in d)a[c]=d[c];for(var f=0;f<ob.length;f++)c=ob[f],Object.prototype.hasOwnProperty.call(d,c)&&(a[c]=d[c])}}
;var qb;function rb(){if(void 0===qb){var a=null,b=w.trustedTypes;if(b&&b.createPolicy){try{a=b.createPolicy("goog#html",{createHTML:Va,createScript:Va,createScriptURL:Va})}catch(c){w.console&&w.console.error(c.message)}qb=a}else qb=a}return qb}
;function sb(a,b){this.f=b===tb?a:""}
sb.prototype.R=!0;sb.prototype.O=function(){return this.f.toString()};
sb.prototype.ma=!0;sb.prototype.ja=function(){return 1};
function ub(a){if(a instanceof sb&&a.constructor===sb)return a.f;La(a);return"type_error:TrustedResourceUrl"}
var tb={};var vb=String.prototype.trim?function(a){return a.trim()}:function(a){return/^[\s\xa0]*([\s\S]*?)[\s\xa0]*$/.exec(a)[1]};
function wb(a,b){if(b)a=a.replace(xb,"&amp;").replace(yb,"&lt;").replace(zb,"&gt;").replace(Ab,"&quot;").replace(Bb,"&#39;").replace(Cb,"&#0;");else{if(!Db.test(a))return a;-1!=a.indexOf("&")&&(a=a.replace(xb,"&amp;"));-1!=a.indexOf("<")&&(a=a.replace(yb,"&lt;"));-1!=a.indexOf(">")&&(a=a.replace(zb,"&gt;"));-1!=a.indexOf('"')&&(a=a.replace(Ab,"&quot;"));-1!=a.indexOf("'")&&(a=a.replace(Bb,"&#39;"));-1!=a.indexOf("\x00")&&(a=a.replace(Cb,"&#0;"))}return a}
var xb=/&/g,yb=/</g,zb=/>/g,Ab=/"/g,Bb=/'/g,Cb=/\x00/g,Db=/[\x00&<>"']/;function Eb(a,b){return a<b?-1:a>b?1:0}
;function I(a,b){this.f=b===Fb?a:""}
I.prototype.R=!0;I.prototype.O=function(){return this.f.toString()};
I.prototype.ma=!0;I.prototype.ja=function(){return 1};
function Gb(a){if(a instanceof I&&a.constructor===I)return a.f;La(a);return"type_error:SafeUrl"}
var Hb=/^(?:audio\/(?:3gpp2|3gpp|aac|L16|midi|mp3|mp4|mpeg|oga|ogg|opus|x-m4a|x-matroska|x-wav|wav|webm)|font\/\w+|image\/(?:bmp|gif|jpeg|jpg|png|tiff|webp|x-icon)|video\/(?:mpeg|mp4|ogg|webm|quicktime|x-matroska))(?:;\w+=(?:\w+|"[\w;,= ]+"))*$/i,Ib=/^data:(.*);base64,[a-z0-9+\/]+=*$/i,Jb=/^(?:(?:https?|mailto|ftp):|[^:/?#]*(?:[/?#]|$))/i;function Kb(a){if(a instanceof I)return a;a="object"==typeof a&&a.R?a.O():String(a);Jb.test(a)||(a="about:invalid#zClosurez");return new I(a,Fb)}
var Fb={},Lb=new I("about:invalid#zClosurez",Fb);var Mb;a:{var Nb=w.navigator;if(Nb){var Ob=Nb.userAgent;if(Ob){Mb=Ob;break a}}Mb=""}function J(a){return-1!=Mb.indexOf(a)}
;function Pb(a,b,c){this.f=c===Qb?a:"";this.g=b}
Pb.prototype.ma=!0;Pb.prototype.ja=function(){return this.g};
Pb.prototype.R=!0;Pb.prototype.O=function(){return this.f.toString()};
var Qb={};function Rb(a,b){var c=rb();c=c?c.createHTML(a):a;return new Pb(c,b,Qb)}
;function Sb(a,b){var c=b instanceof I?b:Kb(b);a.href=Gb(c)}
function Tb(a,b){a.src=ub(b);var c=Ga(a.ownerDocument&&a.ownerDocument.defaultView);c&&a.setAttribute("nonce",c)}
;function Ub(a){return a=wb(a,void 0)}
function Vb(a){for(var b=0,c=0;c<a.length;++c)b=31*b+a.charCodeAt(c)>>>0;return b}
;var Wb=/^(?:([^:/?#.]+):)?(?:\/\/(?:([^\\/?#]*)@)?([^\\/?#]*?)(?::([0-9]+))?(?=[\\/?#]|$))?([^?#]+)?(?:\?([^#]*))?(?:#([\s\S]*))?$/;function Xb(a){return a?decodeURI(a):a}
function K(a){return Xb(a.match(Wb)[3]||null)}
function Yb(a,b,c){if(Array.isArray(b))for(var d=0;d<b.length;d++)Yb(a,String(b[d]),c);else null!=b&&c.push(a+(""===b?"":"="+encodeURIComponent(String(b))))}
function Zb(a){var b=[],c;for(c in a)Yb(c,a[c],b);return b.join("&")}
function ac(a,b){var c=Zb(b);if(c){var d=a.indexOf("#");0>d&&(d=a.length);var e=a.indexOf("?");if(0>e||e>d){e=d;var f=""}else f=a.substring(e+1,d);d=[a.substr(0,e),f,a.substr(d)];e=d[1];d[1]=c?e?e+"&"+c:c:e;c=d[0]+(d[1]?"?"+d[1]:"")+d[2]}else c=a;return c}
var bc=/#|$/;function cc(a,b){var c=void 0;return new (c||(c=Promise))(function(d,e){function f(k){try{h(b.next(k))}catch(l){e(l)}}
function g(k){try{h(b["throw"](k))}catch(l){e(l)}}
function h(k){k.done?d(k.value):(new c(function(l){l(k.value)})).then(f,g)}
h((b=b.apply(a,void 0)).next())})}
;function dc(a){dc[" "](a);return a}
dc[" "]=z;var ec=J("Opera"),fc=J("Trident")||J("MSIE"),gc=J("Edge"),hc=J("Gecko")&&!(-1!=Mb.toLowerCase().indexOf("webkit")&&!J("Edge"))&&!(J("Trident")||J("MSIE"))&&!J("Edge"),ic=-1!=Mb.toLowerCase().indexOf("webkit")&&!J("Edge");function jc(){var a=w.document;return a?a.documentMode:void 0}
var kc;a:{var lc="",mc=function(){var a=Mb;if(hc)return/rv:([^\);]+)(\)|;)/.exec(a);if(gc)return/Edge\/([\d\.]+)/.exec(a);if(fc)return/\b(?:MSIE|rv)[: ]([^\);]+)(\)|;)/.exec(a);if(ic)return/WebKit\/(\S+)/.exec(a);if(ec)return/(?:Version)[ \/]?(\S+)/.exec(a)}();
mc&&(lc=mc?mc[1]:"");if(fc){var nc=jc();if(null!=nc&&nc>parseFloat(lc)){kc=String(nc);break a}}kc=lc}var oc=kc,pc={},qc;if(w.document&&fc){var rc=jc();qc=rc?rc:parseInt(oc,10)||void 0}else qc=void 0;var sc=qc;var tc=J("iPhone")&&!J("iPod")&&!J("iPad")||J("iPod"),uc=J("iPad");var vc={},wc=null;var L=window;function xc(a){var b=y("window.location.href");null==a&&(a='Unknown Error of type "null/undefined"');if("string"===typeof a)return{message:a,name:"Unknown error",lineNumber:"Not available",fileName:b,stack:"Not available"};var c=!1;try{var d=a.lineNumber||a.line||"Not available"}catch(g){d="Not available",c=!0}try{var e=a.fileName||a.filename||a.sourceURL||w.$googDebugFname||b}catch(g){e="Not available",c=!0}b=yc(a);if(!(!c&&a.lineNumber&&a.fileName&&a.stack&&a.message&&a.name)){c=a.message;if(null==
c){if(a.constructor&&a.constructor instanceof Function){if(a.constructor.name)c=a.constructor.name;else if(c=a.constructor,zc[c])c=zc[c];else{c=String(c);if(!zc[c]){var f=/function\s+([^\(]+)/m.exec(c);zc[c]=f?f[1]:"[Anonymous]"}c=zc[c]}c='Unknown Error of type "'+c+'"'}else c="Unknown Error of unknown type";"function"===typeof a.toString&&Object.prototype.toString!==a.toString&&(c+=": "+a.toString())}return{message:c,name:a.name||"UnknownError",lineNumber:d,fileName:e,stack:b||"Not available"}}a.stack=
b;return a}
function yc(a,b){b||(b={});b[Ac(a)]=!0;var c=a.stack||"",d=a.Ab;d&&!b[Ac(d)]&&(c+="\nCaused by: ",d.stack&&0==d.stack.indexOf(d.toString())||(c+="string"===typeof d?d:d.message+"\n"),c+=yc(d,b));return c}
function Ac(a){var b="";"function"===typeof a.toString&&(b=""+a);return b+a.stack}
var zc={};function Bc(a){this.f=a||{cookie:""}}
n=Bc.prototype;n.isEnabled=function(){return navigator.cookieEnabled};
n.set=function(a,b,c){var d=!1;if("object"===typeof c){var e=c.Gb;d=c.secure||!1;var f=c.domain||void 0;var g=c.path||void 0;var h=c.va}if(/[;=\s]/.test(a))throw Error('Invalid cookie name "'+a+'"');if(/[;\r\n]/.test(b))throw Error('Invalid cookie value "'+b+'"');void 0===h&&(h=-1);this.f.cookie=a+"="+b+(f?";domain="+f:"")+(g?";path="+g:"")+(0>h?"":0==h?";expires="+(new Date(1970,1,1)).toUTCString():";expires="+(new Date(Date.now()+1E3*h)).toUTCString())+(d?";secure":"")+(null!=e?";samesite="+e:"")};
n.get=function(a,b){for(var c=a+"=",d=(this.f.cookie||"").split(";"),e=0,f;e<d.length;e++){f=vb(d[e]);if(0==f.lastIndexOf(c,0))return f.substr(c.length);if(f==a)return""}return b};
n.remove=function(a,b,c){var d=void 0!==this.get(a);this.set(a,"",{va:0,path:b,domain:c});return d};
n.isEmpty=function(){return!this.f.cookie};
n.clear=function(){for(var a=(this.f.cookie||"").split(";"),b=[],c=[],d,e,f=0;f<a.length;f++)e=vb(a[f]),d=e.indexOf("="),-1==d?(b.push(""),c.push(e)):(b.push(e.substring(0,d)),c.push(e.substring(d+1)));for(a=b.length-1;0<=a;a--)this.remove(b[a])};
var Cc=new Bc("undefined"==typeof document?null:document);var Dc=!fc||9<=Number(sc);function Ec(a,b){this.x=void 0!==a?a:0;this.y=void 0!==b?b:0}
n=Ec.prototype;n.clone=function(){return new Ec(this.x,this.y)};
n.equals=function(a){return a instanceof Ec&&(this==a?!0:this&&a?this.x==a.x&&this.y==a.y:!1)};
n.ceil=function(){this.x=Math.ceil(this.x);this.y=Math.ceil(this.y);return this};
n.floor=function(){this.x=Math.floor(this.x);this.y=Math.floor(this.y);return this};
n.round=function(){this.x=Math.round(this.x);this.y=Math.round(this.y);return this};function Fc(a,b){this.width=a;this.height=b}
n=Fc.prototype;n.clone=function(){return new Fc(this.width,this.height)};
n.aspectRatio=function(){return this.width/this.height};
n.isEmpty=function(){return!(this.width*this.height)};
n.ceil=function(){this.width=Math.ceil(this.width);this.height=Math.ceil(this.height);return this};
n.floor=function(){this.width=Math.floor(this.width);this.height=Math.floor(this.height);return this};
n.round=function(){this.width=Math.round(this.width);this.height=Math.round(this.height);return this};function Gc(a){var b=document;return"string"===typeof a?b.getElementById(a):a}
function Hc(a,b){fb(b,function(c,d){c&&"object"==typeof c&&c.R&&(c=c.O());"style"==d?a.style.cssText=c:"class"==d?a.className=c:"for"==d?a.htmlFor=c:Ic.hasOwnProperty(d)?a.setAttribute(Ic[d],c):0==d.lastIndexOf("aria-",0)||0==d.lastIndexOf("data-",0)?a.setAttribute(d,c):a[d]=c})}
var Ic={cellpadding:"cellPadding",cellspacing:"cellSpacing",colspan:"colSpan",frameborder:"frameBorder",height:"height",maxlength:"maxLength",nonce:"nonce",role:"role",rowspan:"rowSpan",type:"type",usemap:"useMap",valign:"vAlign",width:"width"};
function Jc(a,b,c){var d=arguments,e=document,f=String(d[0]),g=d[1];if(!Dc&&g&&(g.name||g.type)){f=["<",f];g.name&&f.push(' name="',Ub(g.name),'"');if(g.type){f.push(' type="',Ub(g.type),'"');var h={};pb(h,g);delete h.type;g=h}f.push(">");f=f.join("")}f=Kc(e,f);g&&("string"===typeof g?f.className=g:Array.isArray(g)?f.className=g.join(" "):Hc(f,g));2<d.length&&Lc(e,f,d);return f}
function Lc(a,b,c){function d(h){h&&b.appendChild("string"===typeof h?a.createTextNode(h):h)}
for(var e=2;e<c.length;e++){var f=c[e];if(!Ma(f)||B(f)&&0<f.nodeType)d(f);else{a:{if(f&&"number"==typeof f.length){if(B(f)){var g="function"==typeof f.item||"string"==typeof f.item;break a}if("function"===typeof f){g="function"==typeof f.item;break a}}g=!1}H(g?db(f):f,d)}}}
function Kc(a,b){b=String(b);"application/xhtml+xml"===a.contentType&&(b=b.toLowerCase());return a.createElement(b)}
function Mc(a,b){for(var c=0;a;){if(b(a))return a;a=a.parentNode;c++}return null}
;function Nc(a){var b=Oc;if(b)for(var c in b)Object.prototype.hasOwnProperty.call(b,c)&&a.call(void 0,b[c],c,b)}
function Pc(){var a=[];Nc(function(b){a.push(b)});
return a}
var Oc={mb:"allow-forms",nb:"allow-modals",ob:"allow-orientation-lock",pb:"allow-pointer-lock",qb:"allow-popups",rb:"allow-popups-to-escape-sandbox",sb:"allow-presentation",tb:"allow-same-origin",ub:"allow-scripts",vb:"allow-top-navigation",wb:"allow-top-navigation-by-user-activation"},Qc=Xa(function(){return Pc()});
function Rc(){var a=Kc(document,"IFRAME"),b={};H(Qc(),function(c){a.sandbox&&a.sandbox.supports&&a.sandbox.supports(c)&&(b[c]=!0)});
return b}
;function M(){this.g=this.g;this.A=this.A}
M.prototype.g=!1;M.prototype.dispose=function(){this.g||(this.g=!0,this.o())};
function Sc(a,b){a.g?b():(a.A||(a.A=[]),a.A.push(b))}
M.prototype.o=function(){if(this.A)for(;this.A.length;)this.A.shift()()};
function Tc(a){a&&"function"==typeof a.dispose&&a.dispose()}
function Uc(a){for(var b=0,c=arguments.length;b<c;++b){var d=arguments[b];Ma(d)?Uc.apply(null,d):Tc(d)}}
;var Vc={};function Wc(){}
function Xc(a,b){if(b!==Vc)throw Error("Bad secret");this.f=a}
v(Xc,Wc);Xc.prototype.toString=function(){return this.f};new Xc("about:blank",Vc);new Xc("about:invalid#zTSz",Vc);function Yc(a){Zc();var b=rb();a=b?b.createScriptURL(a):a;return new sb(a,tb)}
var Zc=z;function $c(a){"number"==typeof a&&(a=Math.round(a)+"px");return a}
;var ad=(new Date).getTime();function bd(a){if(!a)return"";a=a.split("#")[0].split("?")[0];a=a.toLowerCase();0==a.indexOf("//")&&(a=window.location.protocol+a);/^[\w\-]*:\/\//.test(a)||(a=window.location.href);var b=a.substring(a.indexOf("://")+3),c=b.indexOf("/");-1!=c&&(b=b.substring(0,c));a=a.substring(0,a.indexOf("://"));if("http"!==a&&"https"!==a&&"chrome-extension"!==a&&"moz-extension"!==a&&"file"!==a&&"android-app"!==a&&"chrome-search"!==a&&"chrome-untrusted"!==a&&"chrome"!==a&&"app"!==a&&"devtools"!==a)throw Error("Invalid URI scheme in origin: "+
a);c="";var d=b.indexOf(":");if(-1!=d){var e=b.substring(d+1);b=b.substring(0,d);if("http"===a&&"80"!==e||"https"===a&&"443"!==e)c=":"+e}return a+"://"+b+c}
;function cd(){function a(){e[0]=1732584193;e[1]=4023233417;e[2]=2562383102;e[3]=271733878;e[4]=3285377520;m=l=0}
function b(p){for(var u=g,r=0;64>r;r+=4)u[r/4]=p[r]<<24|p[r+1]<<16|p[r+2]<<8|p[r+3];for(r=16;80>r;r++)p=u[r-3]^u[r-8]^u[r-14]^u[r-16],u[r]=(p<<1|p>>>31)&4294967295;p=e[0];var A=e[1],D=e[2],T=e[3],td=e[4];for(r=0;80>r;r++){if(40>r)if(20>r){var Ta=T^A&(D^T);var $b=1518500249}else Ta=A^D^T,$b=1859775393;else 60>r?(Ta=A&D|T&(A|D),$b=2400959708):(Ta=A^D^T,$b=3395469782);Ta=((p<<5|p>>>27)&4294967295)+Ta+td+$b+u[r]&4294967295;td=T;T=D;D=(A<<30|A>>>2)&4294967295;A=p;p=Ta}e[0]=e[0]+p&4294967295;e[1]=e[1]+
A&4294967295;e[2]=e[2]+D&4294967295;e[3]=e[3]+T&4294967295;e[4]=e[4]+td&4294967295}
function c(p,u){if("string"===typeof p){p=unescape(encodeURIComponent(p));for(var r=[],A=0,D=p.length;A<D;++A)r.push(p.charCodeAt(A));p=r}u||(u=p.length);r=0;if(0==l)for(;r+64<u;)b(p.slice(r,r+64)),r+=64,m+=64;for(;r<u;)if(f[l++]=p[r++],m++,64==l)for(l=0,b(f);r+64<u;)b(p.slice(r,r+64)),r+=64,m+=64}
function d(){var p=[],u=8*m;56>l?c(h,56-l):c(h,64-(l-56));for(var r=63;56<=r;r--)f[r]=u&255,u>>>=8;b(f);for(r=u=0;5>r;r++)for(var A=24;0<=A;A-=8)p[u++]=e[r]>>A&255;return p}
for(var e=[],f=[],g=[],h=[128],k=1;64>k;++k)h[k]=0;var l,m;a();return{reset:a,update:c,digest:d,Ha:function(){for(var p=d(),u="",r=0;r<p.length;r++)u+="0123456789ABCDEF".charAt(Math.floor(p[r]/16))+"0123456789ABCDEF".charAt(p[r]%16);return u}}}
;function dd(a,b,c){var d=[],e=[];if(1==(Array.isArray(c)?2:1))return e=[b,a],H(d,function(h){e.push(h)}),ed(e.join(" "));
var f=[],g=[];H(c,function(h){g.push(h.key);f.push(h.value)});
c=Math.floor((new Date).getTime()/1E3);e=0==f.length?[c,b,a]:[f.join(":"),c,b,a];H(d,function(h){e.push(h)});
a=ed(e.join(" "));a=[c,a];0==g.length||a.push(g.join(""));return a.join("_")}
function ed(a){var b=cd();b.update(a);return b.Ha().toLowerCase()}
;function fd(a){var b=bd(String(w.location.href)),c;(c=w.__SAPISID||w.__APISID||w.__OVERRIDE_SID)?c=!0:(c=new Bc(document),c=c.get("SAPISID")||c.get("APISID")||c.get("__Secure-3PAPISID")||c.get("SID"),c=!!c);if(c&&(c=(b=0==b.indexOf("https:")||0==b.indexOf("chrome-extension:")||0==b.indexOf("moz-extension:"))?w.__SAPISID:w.__APISID,c||(c=new Bc(document),c=c.get(b?"SAPISID":"APISID")||c.get("__Secure-3PAPISID")),c)){b=b?"SAPISIDHASH":"APISIDHASH";var d=String(w.location.href);return d&&c&&b?[b,dd(bd(d),
c,a||null)].join(" "):null}return null}
;function gd(){this.g=[];this.f=-1}
gd.prototype.set=function(a,b){b=void 0===b?!0:b;0<=a&&52>a&&0===a%1&&this.g[a]!=b&&(this.g[a]=b,this.f=-1)};
gd.prototype.get=function(a){return!!this.g[a]};
function hd(a){-1==a.f&&(a.f=ab(a.g,function(b,c,d){return c?b+Math.pow(2,d):b},0));
return a.f}
;function id(a,b){this.h=a;this.i=b;this.g=0;this.f=null}
id.prototype.get=function(){if(0<this.g){this.g--;var a=this.f;this.f=a.next;a.next=null}else a=this.h();return a};
function jd(a,b){a.i(b);100>a.g&&(a.g++,b.next=a.f,a.f=b)}
;function kd(a){w.setTimeout(function(){throw a;},0)}
var ld;
function md(){var a=w.MessageChannel;"undefined"===typeof a&&"undefined"!==typeof window&&window.postMessage&&window.addEventListener&&!J("Presto")&&(a=function(){var e=Kc(document,"IFRAME");e.style.display="none";document.documentElement.appendChild(e);var f=e.contentWindow;e=f.document;e.open();e.close();var g="callImmediate"+Math.random(),h="file:"==f.location.protocol?"*":f.location.protocol+"//"+f.location.host;e=C(function(k){if(("*"==h||k.origin==h)&&k.data==g)this.port1.onmessage()},this);
f.addEventListener("message",e,!1);this.port1={};this.port2={postMessage:function(){f.postMessage(g,h)}}});
if("undefined"!==typeof a&&!J("Trident")&&!J("MSIE")){var b=new a,c={},d=c;b.port1.onmessage=function(){if(void 0!==c.next){c=c.next;var e=c.ta;c.ta=null;e()}};
return function(e){d.next={ta:e};d=d.next;b.port2.postMessage(0)}}return function(e){w.setTimeout(e,0)}}
;function nd(){this.g=this.f=null}
var pd=new id(function(){return new od},function(a){a.reset()});
nd.prototype.add=function(a,b){var c=pd.get();c.set(a,b);this.g?this.g.next=c:this.f=c;this.g=c};
nd.prototype.remove=function(){var a=null;this.f&&(a=this.f,this.f=this.f.next,this.f||(this.g=null),a.next=null);return a};
function od(){this.next=this.scope=this.f=null}
od.prototype.set=function(a,b){this.f=a;this.scope=b;this.next=null};
od.prototype.reset=function(){this.next=this.scope=this.f=null};function qd(a,b){rd||sd();ud||(rd(),ud=!0);vd.add(a,b)}
var rd;function sd(){if(w.Promise&&w.Promise.resolve){var a=w.Promise.resolve(void 0);rd=function(){a.then(wd)}}else rd=function(){var b=wd;
"function"!==typeof w.setImmediate||w.Window&&w.Window.prototype&&!J("Edge")&&w.Window.prototype.setImmediate==w.setImmediate?(ld||(ld=md()),ld(b)):w.setImmediate(b)}}
var ud=!1,vd=new nd;function wd(){for(var a;a=vd.remove();){try{a.f.call(a.scope)}catch(b){kd(b)}jd(pd,a)}ud=!1}
;function xd(){this.g=-1}
;function yd(){this.g=64;this.f=[];this.l=[];this.m=[];this.i=[];this.i[0]=128;for(var a=1;a<this.g;++a)this.i[a]=0;this.j=this.h=0;this.reset()}
F(yd,xd);yd.prototype.reset=function(){this.f[0]=1732584193;this.f[1]=4023233417;this.f[2]=2562383102;this.f[3]=271733878;this.f[4]=3285377520;this.j=this.h=0};
function zd(a,b,c){c||(c=0);var d=a.m;if("string"===typeof b)for(var e=0;16>e;e++)d[e]=b.charCodeAt(c)<<24|b.charCodeAt(c+1)<<16|b.charCodeAt(c+2)<<8|b.charCodeAt(c+3),c+=4;else for(e=0;16>e;e++)d[e]=b[c]<<24|b[c+1]<<16|b[c+2]<<8|b[c+3],c+=4;for(e=16;80>e;e++){var f=d[e-3]^d[e-8]^d[e-14]^d[e-16];d[e]=(f<<1|f>>>31)&4294967295}b=a.f[0];c=a.f[1];var g=a.f[2],h=a.f[3],k=a.f[4];for(e=0;80>e;e++){if(40>e)if(20>e){f=h^c&(g^h);var l=1518500249}else f=c^g^h,l=1859775393;else 60>e?(f=c&g|h&(c|g),l=2400959708):
(f=c^g^h,l=3395469782);f=(b<<5|b>>>27)+f+k+l+d[e]&4294967295;k=h;h=g;g=(c<<30|c>>>2)&4294967295;c=b;b=f}a.f[0]=a.f[0]+b&4294967295;a.f[1]=a.f[1]+c&4294967295;a.f[2]=a.f[2]+g&4294967295;a.f[3]=a.f[3]+h&4294967295;a.f[4]=a.f[4]+k&4294967295}
yd.prototype.update=function(a,b){if(null!=a){void 0===b&&(b=a.length);for(var c=b-this.g,d=0,e=this.l,f=this.h;d<b;){if(0==f)for(;d<=c;)zd(this,a,d),d+=this.g;if("string"===typeof a)for(;d<b;){if(e[f]=a.charCodeAt(d),++f,++d,f==this.g){zd(this,e);f=0;break}}else for(;d<b;)if(e[f]=a[d],++f,++d,f==this.g){zd(this,e);f=0;break}}this.h=f;this.j+=b}};
yd.prototype.digest=function(){var a=[],b=8*this.j;56>this.h?this.update(this.i,56-this.h):this.update(this.i,this.g-(this.h-56));for(var c=this.g-1;56<=c;c--)this.l[c]=b&255,b/=256;zd(this,this.l);for(c=b=0;5>c;c++)for(var d=24;0<=d;d-=8)a[b]=this.f[c]>>d&255,++b;return a};var Ad="StopIteration"in w?w.StopIteration:{message:"StopIteration",stack:""};function Bd(){}
Bd.prototype.next=function(){throw Ad;};
Bd.prototype.J=function(){return this};
function Cd(a){if(a instanceof Bd)return a;if("function"==typeof a.J)return a.J(!1);if(Ma(a)){var b=0,c=new Bd;c.next=function(){for(;;){if(b>=a.length)throw Ad;if(b in a)return a[b++];b++}};
return c}throw Error("Not implemented");}
function Dd(a,b){if(Ma(a))try{H(a,b,void 0)}catch(c){if(c!==Ad)throw c;}else{a=Cd(a);try{for(;;)b.call(void 0,a.next(),void 0,a)}catch(c){if(c!==Ad)throw c;}}}
function Ed(a){if(Ma(a))return db(a);a=Cd(a);var b=[];Dd(a,function(c){b.push(c)});
return b}
;function Fd(a,b){this.h={};this.f=[];this.K=this.g=0;var c=arguments.length;if(1<c){if(c%2)throw Error("Uneven number of arguments");for(var d=0;d<c;d+=2)this.set(arguments[d],arguments[d+1])}else if(a)if(a instanceof Fd)for(c=Gd(a),d=0;d<c.length;d++)this.set(c[d],a.get(c[d]));else for(d in a)this.set(d,a[d])}
function Gd(a){Hd(a);return a.f.concat()}
n=Fd.prototype;n.equals=function(a,b){if(this===a)return!0;if(this.g!=a.g)return!1;var c=b||Id;Hd(this);for(var d,e=0;d=this.f[e];e++)if(!c(this.get(d),a.get(d)))return!1;return!0};
function Id(a,b){return a===b}
n.isEmpty=function(){return 0==this.g};
n.clear=function(){this.h={};this.K=this.g=this.f.length=0};
n.remove=function(a){return Object.prototype.hasOwnProperty.call(this.h,a)?(delete this.h[a],this.g--,this.K++,this.f.length>2*this.g&&Hd(this),!0):!1};
function Hd(a){if(a.g!=a.f.length){for(var b=0,c=0;b<a.f.length;){var d=a.f[b];Object.prototype.hasOwnProperty.call(a.h,d)&&(a.f[c++]=d);b++}a.f.length=c}if(a.g!=a.f.length){var e={};for(c=b=0;b<a.f.length;)d=a.f[b],Object.prototype.hasOwnProperty.call(e,d)||(a.f[c++]=d,e[d]=1),b++;a.f.length=c}}
n.get=function(a,b){return Object.prototype.hasOwnProperty.call(this.h,a)?this.h[a]:b};
n.set=function(a,b){Object.prototype.hasOwnProperty.call(this.h,a)||(this.g++,this.f.push(a),this.K++);this.h[a]=b};
n.forEach=function(a,b){for(var c=Gd(this),d=0;d<c.length;d++){var e=c[d],f=this.get(e);a.call(b,f,e,this)}};
n.clone=function(){return new Fd(this)};
n.J=function(a){Hd(this);var b=0,c=this.K,d=this,e=new Bd;e.next=function(){if(c!=d.K)throw Error("The map has changed since the iterator was created");if(b>=d.f.length)throw Ad;var f=d.f[b++];return a?f:d.h[f]};
return e};function Jd(a){return"string"==typeof a.className?a.className:a.getAttribute&&a.getAttribute("class")||""}
function Kd(a,b){"string"==typeof a.className?a.className=b:a.setAttribute&&a.setAttribute("class",b)}
function Ld(a,b){if(a.classList)var c=a.classList.contains(b);else c=a.classList?a.classList:Jd(a).match(/\S+/g)||[],c=0<=Ya(c,b);return c}
function Md(){var a=document.body;a.classList?a.classList.remove("inverted-hdpi"):Ld(a,"inverted-hdpi")&&Kd(a,Za(a.classList?a.classList:Jd(a).match(/\S+/g)||[],function(b){return"inverted-hdpi"!=b}).join(" "))}
;var Nd=!fc||9<=Number(sc),Od;
if(Od=fc){var Pd;if(Object.prototype.hasOwnProperty.call(pc,"9"))Pd=pc["9"];else{for(var Qd=0,Rd=vb(String(oc)).split("."),Sd=vb("9").split("."),Td=Math.max(Rd.length,Sd.length),Ud=0;0==Qd&&Ud<Td;Ud++){var Vd=Rd[Ud]||"",Wd=Sd[Ud]||"";do{var Xd=/(\d*)(\D*)(.*)/.exec(Vd)||["","","",""],Yd=/(\d*)(\D*)(.*)/.exec(Wd)||["","","",""];if(0==Xd[0].length&&0==Yd[0].length)break;Qd=Eb(0==Xd[1].length?0:parseInt(Xd[1],10),0==Yd[1].length?0:parseInt(Yd[1],10))||Eb(0==Xd[2].length,0==Yd[2].length)||Eb(Xd[2],Yd[2]);
Vd=Xd[3];Wd=Yd[3]}while(0==Qd)}Pd=pc["9"]=0<=Qd}Od=!Pd}var Zd=Od,$d=function(){if(!w.addEventListener||!Object.defineProperty)return!1;var a=!1,b=Object.defineProperty({},"passive",{get:function(){a=!0}});
try{w.addEventListener("test",z,b),w.removeEventListener("test",z,b)}catch(c){}return a}();function ae(a,b){this.type=a;this.f=this.target=b;this.defaultPrevented=this.g=!1}
ae.prototype.stopPropagation=function(){this.g=!0};
ae.prototype.preventDefault=function(){this.defaultPrevented=!0};function be(a,b){ae.call(this,a?a.type:"");this.relatedTarget=this.f=this.target=null;this.button=this.screenY=this.screenX=this.clientY=this.clientX=0;this.key="";this.charCode=this.keyCode=0;this.metaKey=this.shiftKey=this.altKey=this.ctrlKey=!1;this.state=null;this.pointerId=0;this.pointerType="";this.h=null;a&&this.init(a,b)}
F(be,ae);var ce={2:"touch",3:"pen",4:"mouse"};
be.prototype.init=function(a,b){var c=this.type=a.type,d=a.changedTouches&&a.changedTouches.length?a.changedTouches[0]:null;this.target=a.target||a.srcElement;this.f=b;var e=a.relatedTarget;if(e){if(hc){a:{try{dc(e.nodeName);var f=!0;break a}catch(g){}f=!1}f||(e=null)}}else"mouseover"==c?e=a.fromElement:"mouseout"==c&&(e=a.toElement);this.relatedTarget=e;d?(this.clientX=void 0!==d.clientX?d.clientX:d.pageX,this.clientY=void 0!==d.clientY?d.clientY:d.pageY,this.screenX=d.screenX||0,this.screenY=d.screenY||
0):(this.clientX=void 0!==a.clientX?a.clientX:a.pageX,this.clientY=void 0!==a.clientY?a.clientY:a.pageY,this.screenX=a.screenX||0,this.screenY=a.screenY||0);this.button=a.button;this.keyCode=a.keyCode||0;this.key=a.key||"";this.charCode=a.charCode||("keypress"==c?a.keyCode:0);this.ctrlKey=a.ctrlKey;this.altKey=a.altKey;this.shiftKey=a.shiftKey;this.metaKey=a.metaKey;this.pointerId=a.pointerId||0;this.pointerType="string"===typeof a.pointerType?a.pointerType:ce[a.pointerType]||"";this.state=a.state;
this.h=a;a.defaultPrevented&&this.preventDefault()};
be.prototype.stopPropagation=function(){be.I.stopPropagation.call(this);this.h.stopPropagation?this.h.stopPropagation():this.h.cancelBubble=!0};
be.prototype.preventDefault=function(){be.I.preventDefault.call(this);var a=this.h;if(a.preventDefault)a.preventDefault();else if(a.returnValue=!1,Zd)try{if(a.ctrlKey||112<=a.keyCode&&123>=a.keyCode)a.keyCode=-1}catch(b){}};var de="closure_listenable_"+(1E6*Math.random()|0),ee=0;function fe(a,b,c,d,e){this.listener=a;this.f=null;this.src=b;this.type=c;this.capture=!!d;this.fa=e;this.key=++ee;this.S=this.da=!1}
function ge(a){a.S=!0;a.listener=null;a.f=null;a.src=null;a.fa=null}
;function he(a){this.src=a;this.listeners={};this.f=0}
he.prototype.add=function(a,b,c,d,e){var f=a.toString();a=this.listeners[f];a||(a=this.listeners[f]=[],this.f++);var g=ie(a,b,d,e);-1<g?(b=a[g],c||(b.da=!1)):(b=new fe(b,this.src,f,!!d,e),b.da=c,a.push(b));return b};
he.prototype.remove=function(a,b,c,d){a=a.toString();if(!(a in this.listeners))return!1;var e=this.listeners[a];b=ie(e,b,c,d);return-1<b?(ge(e[b]),Array.prototype.splice.call(e,b,1),0==e.length&&(delete this.listeners[a],this.f--),!0):!1};
function je(a,b){var c=b.type;c in a.listeners&&cb(a.listeners[c],b)&&(ge(b),0==a.listeners[c].length&&(delete a.listeners[c],a.f--))}
function ie(a,b,c,d){for(var e=0;e<a.length;++e){var f=a[e];if(!f.S&&f.listener==b&&f.capture==!!c&&f.fa==d)return e}return-1}
;var ke="closure_lm_"+(1E6*Math.random()|0),le={},me=0;function ne(a,b,c,d,e){if(d&&d.once)oe(a,b,c,d,e);else if(Array.isArray(b))for(var f=0;f<b.length;f++)ne(a,b[f],c,d,e);else c=pe(c),a&&a[de]?a.f.add(String(b),c,!1,B(d)?!!d.capture:!!d,e):qe(a,b,c,!1,d,e)}
function qe(a,b,c,d,e,f){if(!b)throw Error("Invalid event type");var g=B(e)?!!e.capture:!!e,h=re(a);h||(a[ke]=h=new he(a));c=h.add(b,c,d,g,f);if(!c.f){d=se();c.f=d;d.src=a;d.listener=c;if(a.addEventListener)$d||(e=g),void 0===e&&(e=!1),a.addEventListener(b.toString(),d,e);else if(a.attachEvent)a.attachEvent(te(b.toString()),d);else if(a.addListener&&a.removeListener)a.addListener(d);else throw Error("addEventListener and attachEvent are unavailable.");me++}}
function se(){var a=ue,b=Nd?function(c){return a.call(b.src,b.listener,c)}:function(c){c=a.call(b.src,b.listener,c);
if(!c)return c};
return b}
function oe(a,b,c,d,e){if(Array.isArray(b))for(var f=0;f<b.length;f++)oe(a,b[f],c,d,e);else c=pe(c),a&&a[de]?a.f.add(String(b),c,!0,B(d)?!!d.capture:!!d,e):qe(a,b,c,!0,d,e)}
function ve(a,b,c,d,e){if(Array.isArray(b))for(var f=0;f<b.length;f++)ve(a,b[f],c,d,e);else(d=B(d)?!!d.capture:!!d,c=pe(c),a&&a[de])?a.f.remove(String(b),c,d,e):a&&(a=re(a))&&(b=a.listeners[b.toString()],a=-1,b&&(a=ie(b,c,d,e)),(c=-1<a?b[a]:null)&&we(c))}
function we(a){if("number"!==typeof a&&a&&!a.S){var b=a.src;if(b&&b[de])je(b.f,a);else{var c=a.type,d=a.f;b.removeEventListener?b.removeEventListener(c,d,a.capture):b.detachEvent?b.detachEvent(te(c),d):b.addListener&&b.removeListener&&b.removeListener(d);me--;(c=re(b))?(je(c,a),0==c.f&&(c.src=null,b[ke]=null)):ge(a)}}}
function te(a){return a in le?le[a]:le[a]="on"+a}
function xe(a,b,c,d){var e=!0;if(a=re(a))if(b=a.listeners[b.toString()])for(b=b.concat(),a=0;a<b.length;a++){var f=b[a];f&&f.capture==c&&!f.S&&(f=ye(f,d),e=e&&!1!==f)}return e}
function ye(a,b){var c=a.listener,d=a.fa||a.src;a.da&&we(a);return c.call(d,b)}
function ue(a,b){if(a.S)return!0;if(!Nd){var c=b||y("window.event"),d=new be(c,this),e=!0;if(!(0>c.keyCode||void 0!=c.returnValue)){a:{var f=!1;if(0==c.keyCode)try{c.keyCode=-1;break a}catch(k){f=!0}if(f||void 0==c.returnValue)c.returnValue=!0}c=[];for(f=d.f;f;f=f.parentNode)c.push(f);f=a.type;for(var g=c.length-1;!d.g&&0<=g;g--){d.f=c[g];var h=xe(c[g],f,!0,d);e=e&&h}for(g=0;!d.g&&g<c.length;g++)d.f=c[g],h=xe(c[g],f,!1,d),e=e&&h}return e}return ye(a,new be(b,this))}
function re(a){a=a[ke];return a instanceof he?a:null}
var ze="__closure_events_fn_"+(1E9*Math.random()>>>0);function pe(a){if("function"===typeof a)return a;a[ze]||(a[ze]=function(b){return a.handleEvent(b)});
return a[ze]}
;function Ae(){M.call(this);this.f=new he(this);this.j=this;this.i=null}
F(Ae,M);Ae.prototype[de]=!0;Ae.prototype.addEventListener=function(a,b,c,d){ne(this,a,b,c,d)};
Ae.prototype.removeEventListener=function(a,b,c,d){ve(this,a,b,c,d)};
Ae.prototype.dispatchEvent=function(a){var b=this.i;if(b){var c=[];for(var d=1;b;b=b.i)c.push(b),++d}b=this.j;d=a.type||a;if("string"===typeof a)a=new ae(a,b);else if(a instanceof ae)a.target=a.target||b;else{var e=a;a=new ae(d,b);pb(a,e)}e=!0;if(c)for(var f=c.length-1;!a.g&&0<=f;f--){var g=a.f=c[f];e=Be(g,d,!0,a)&&e}a.g||(g=a.f=b,e=Be(g,d,!0,a)&&e,a.g||(e=Be(g,d,!1,a)&&e));if(c)for(f=0;!a.g&&f<c.length;f++)g=a.f=c[f],e=Be(g,d,!1,a)&&e;return e};
Ae.prototype.o=function(){Ae.I.o.call(this);if(this.f){var a=this.f,b=0,c;for(c in a.listeners){for(var d=a.listeners[c],e=0;e<d.length;e++)++b,ge(d[e]);delete a.listeners[c];a.f--}}this.i=null};
function Be(a,b,c,d){b=a.f.listeners[String(b)];if(!b)return!0;b=b.concat();for(var e=!0,f=0;f<b.length;++f){var g=b[f];if(g&&!g.S&&g.capture==c){var h=g.listener,k=g.fa||g.src;g.da&&je(a.f,g);e=!1!==h.call(k,d)&&e}}return e&&!d.defaultPrevented}
;function Ce(a){var b=[];De(new Ee,a,b);return b.join("")}
function Ee(){}
function De(a,b,c){if(null==b)c.push("null");else{if("object"==typeof b){if(Array.isArray(b)){var d=b;b=d.length;c.push("[");for(var e="",f=0;f<b;f++)c.push(e),De(a,d[f],c),e=",";c.push("]");return}if(b instanceof String||b instanceof Number||b instanceof Boolean)b=b.valueOf();else{c.push("{");e="";for(d in b)Object.prototype.hasOwnProperty.call(b,d)&&(f=b[d],"function"!=typeof f&&(c.push(e),Fe(d,c),c.push(":"),De(a,f,c),e=","));c.push("}");return}}switch(typeof b){case "string":Fe(b,c);break;case "number":c.push(isFinite(b)&&
!isNaN(b)?String(b):"null");break;case "boolean":c.push(String(b));break;case "function":c.push("null");break;default:throw Error("Unknown type: "+typeof b);}}}
var Ge={'"':'\\"',"\\":"\\\\","/":"\\/","\b":"\\b","\f":"\\f","\n":"\\n","\r":"\\r","\t":"\\t","\x0B":"\\u000b"},He=/\uffff/.test("\uffff")?/[\\"\x00-\x1f\x7f-\uffff]/g:/[\\"\x00-\x1f\x7f-\xff]/g;function Fe(a,b){b.push('"',a.replace(He,function(c){var d=Ge[c];d||(d="\\u"+(c.charCodeAt(0)|65536).toString(16).substr(1),Ge[c]=d);return d}),'"')}
;function Ie(a){if(!a)return!1;try{return!!a.$goog_Thenable}catch(b){return!1}}
;function N(a){this.f=0;this.m=void 0;this.i=this.g=this.h=null;this.j=this.l=!1;if(a!=z)try{var b=this;a.call(void 0,function(c){Je(b,2,c)},function(c){Je(b,3,c)})}catch(c){Je(this,3,c)}}
function Ke(){this.next=this.context=this.onRejected=this.g=this.f=null;this.h=!1}
Ke.prototype.reset=function(){this.context=this.onRejected=this.g=this.f=null;this.h=!1};
var Le=new id(function(){return new Ke},function(a){a.reset()});
function Me(a,b,c){var d=Le.get();d.g=a;d.onRejected=b;d.context=c;return d}
function Ne(a){if(a instanceof N)return a;var b=new N(z);Je(b,2,a);return b}
function Oe(a){return new N(function(b,c){c(a)})}
function Pe(a,b,c){Qe(a,b,c,null)||qd(Sa(b,a))}
function Re(a){return new N(function(b,c){var d=a.length,e=[];if(d)for(var f=function(l,m){d--;e[l]=m;0==d&&b(e)},g=function(l){c(l)},h=0,k;h<a.length;h++)k=a[h],Pe(k,Sa(f,h),g);
else b(e)})}
N.prototype.then=function(a,b,c){return Se(this,"function"===typeof a?a:null,"function"===typeof b?b:null,c)};
N.prototype.$goog_Thenable=!0;function Te(a,b){return Se(a,null,b,void 0)}
N.prototype.cancel=function(a){if(0==this.f){var b=new Ue(a);qd(function(){Ve(this,b)},this)}};
function Ve(a,b){if(0==a.f)if(a.h){var c=a.h;if(c.g){for(var d=0,e=null,f=null,g=c.g;g&&(g.h||(d++,g.f==a&&(e=g),!(e&&1<d)));g=g.next)e||(f=g);e&&(0==c.f&&1==d?Ve(c,b):(f?(d=f,d.next==c.i&&(c.i=d),d.next=d.next.next):We(c),Xe(c,e,3,b)))}a.h=null}else Je(a,3,b)}
function Ye(a,b){a.g||2!=a.f&&3!=a.f||Ze(a);a.i?a.i.next=b:a.g=b;a.i=b}
function Se(a,b,c,d){var e=Me(null,null,null);e.f=new N(function(f,g){e.g=b?function(h){try{var k=b.call(d,h);f(k)}catch(l){g(l)}}:f;
e.onRejected=c?function(h){try{var k=c.call(d,h);void 0===k&&h instanceof Ue?g(h):f(k)}catch(l){g(l)}}:g});
e.f.h=a;Ye(a,e);return e.f}
N.prototype.u=function(a){this.f=0;Je(this,2,a)};
N.prototype.C=function(a){this.f=0;Je(this,3,a)};
function Je(a,b,c){0==a.f&&(a===c&&(b=3,c=new TypeError("Promise cannot resolve to itself")),a.f=1,Qe(c,a.u,a.C,a)||(a.m=c,a.f=b,a.h=null,Ze(a),3!=b||c instanceof Ue||$e(a,c)))}
function Qe(a,b,c,d){if(a instanceof N)return Ye(a,Me(b||z,c||null,d)),!0;if(Ie(a))return a.then(b,c,d),!0;if(B(a))try{var e=a.then;if("function"===typeof e)return af(a,e,b,c,d),!0}catch(f){return c.call(d,f),!0}return!1}
function af(a,b,c,d,e){function f(k){h||(h=!0,d.call(e,k))}
function g(k){h||(h=!0,c.call(e,k))}
var h=!1;try{b.call(a,g,f)}catch(k){f(k)}}
function Ze(a){a.l||(a.l=!0,qd(a.A,a))}
function We(a){var b=null;a.g&&(b=a.g,a.g=b.next,b.next=null);a.g||(a.i=null);return b}
N.prototype.A=function(){for(var a;a=We(this);)Xe(this,a,this.f,this.m);this.l=!1};
function Xe(a,b,c,d){if(3==c&&b.onRejected&&!b.h)for(;a&&a.j;a=a.h)a.j=!1;if(b.f)b.f.h=null,bf(b,c,d);else try{b.h?b.g.call(b.context):bf(b,c,d)}catch(e){cf.call(null,e)}jd(Le,b)}
function bf(a,b,c){2==b?a.g.call(a.context,c):a.onRejected&&a.onRejected.call(a.context,c)}
function $e(a,b){a.j=!0;qd(function(){a.j&&cf.call(null,b)})}
var cf=kd;function Ue(a){G.call(this,a)}
F(Ue,G);Ue.prototype.name="cancel";function O(a){M.call(this);this.l=1;this.i=[];this.j=0;this.f=[];this.h={};this.m=!!a}
F(O,M);n=O.prototype;n.subscribe=function(a,b,c){var d=this.h[a];d||(d=this.h[a]=[]);var e=this.l;this.f[e]=a;this.f[e+1]=b;this.f[e+2]=c;this.l=e+3;d.push(e);return e};
function df(a,b,c,d){if(b=a.h[b]){var e=a.f;(b=bb(b,function(f){return e[f+1]==c&&e[f+2]==d}))&&a.T(b)}}
n.T=function(a){var b=this.f[a];if(b){var c=this.h[b];0!=this.j?(this.i.push(a),this.f[a+1]=z):(c&&cb(c,a),delete this.f[a],delete this.f[a+1],delete this.f[a+2])}return!!b};
n.P=function(a,b){var c=this.h[a];if(c){for(var d=Array(arguments.length-1),e=1,f=arguments.length;e<f;e++)d[e-1]=arguments[e];if(this.m)for(e=0;e<c.length;e++){var g=c[e];ef(this.f[g+1],this.f[g+2],d)}else{this.j++;try{for(e=0,f=c.length;e<f;e++)g=c[e],this.f[g+1].apply(this.f[g+2],d)}finally{if(this.j--,0<this.i.length&&0==this.j)for(;c=this.i.pop();)this.T(c)}}return 0!=e}return!1};
function ef(a,b,c){qd(function(){a.apply(b,c)})}
n.clear=function(a){if(a){var b=this.h[a];b&&(H(b,this.T,this),delete this.h[a])}else this.f.length=0,this.h={}};
n.o=function(){O.I.o.call(this);this.clear();this.i.length=0};function ff(a){this.f=a}
ff.prototype.set=function(a,b){void 0===b?this.f.remove(a):this.f.set(a,Ce(b))};
ff.prototype.get=function(a){try{var b=this.f.get(a)}catch(c){return}if(null!==b)try{return JSON.parse(b)}catch(c){throw"Storage: Invalid value was encountered";}};
ff.prototype.remove=function(a){this.f.remove(a)};function gf(a){this.f=a}
F(gf,ff);function hf(a){this.data=a}
function jf(a){return void 0===a||a instanceof hf?a:new hf(a)}
gf.prototype.set=function(a,b){gf.I.set.call(this,a,jf(b))};
gf.prototype.g=function(a){a=gf.I.get.call(this,a);if(void 0===a||a instanceof Object)return a;throw"Storage: Invalid value was encountered";};
gf.prototype.get=function(a){if(a=this.g(a)){if(a=a.data,void 0===a)throw"Storage: Invalid value was encountered";}else a=void 0;return a};function kf(a){this.f=a}
F(kf,gf);kf.prototype.set=function(a,b,c){if(b=jf(b)){if(c){if(c<E()){kf.prototype.remove.call(this,a);return}b.expiration=c}b.creation=E()}kf.I.set.call(this,a,b)};
kf.prototype.g=function(a){var b=kf.I.g.call(this,a);if(b){var c=b.creation,d=b.expiration;if(d&&d<E()||c&&c>E())kf.prototype.remove.call(this,a);else return b}};function lf(){}
;function mf(){}
F(mf,lf);mf.prototype.clear=function(){var a=Ed(this.J(!0)),b=this;H(a,function(c){b.remove(c)})};function nf(a){this.f=a}
F(nf,mf);n=nf.prototype;n.isAvailable=function(){if(!this.f)return!1;try{return this.f.setItem("__sak","1"),this.f.removeItem("__sak"),!0}catch(a){return!1}};
n.set=function(a,b){try{this.f.setItem(a,b)}catch(c){if(0==this.f.length)throw"Storage mechanism: Storage disabled";throw"Storage mechanism: Quota exceeded";}};
n.get=function(a){a=this.f.getItem(a);if("string"!==typeof a&&null!==a)throw"Storage mechanism: Invalid value was encountered";return a};
n.remove=function(a){this.f.removeItem(a)};
n.J=function(a){var b=0,c=this.f,d=new Bd;d.next=function(){if(b>=c.length)throw Ad;var e=c.key(b++);if(a)return e;e=c.getItem(e);if("string"!==typeof e)throw"Storage mechanism: Invalid value was encountered";return e};
return d};
n.clear=function(){this.f.clear()};
n.key=function(a){return this.f.key(a)};function of(){var a=null;try{a=window.localStorage||null}catch(b){}this.f=a}
F(of,nf);function pf(a,b){this.g=a;this.f=null;if(fc&&!(9<=Number(sc))){qf||(qf=new Fd);this.f=qf.get(a);this.f||(b?this.f=document.getElementById(b):(this.f=document.createElement("userdata"),this.f.addBehavior("#default#userData"),document.body.appendChild(this.f)),qf.set(a,this.f));try{this.f.load(this.g)}catch(c){this.f=null}}}
F(pf,mf);var rf={".":".2E","!":".21","~":".7E","*":".2A","'":".27","(":".28",")":".29","%":"."},qf=null;function sf(a){return"_"+encodeURIComponent(a).replace(/[.!~*'()%]/g,function(b){return rf[b]})}
n=pf.prototype;n.isAvailable=function(){return!!this.f};
n.set=function(a,b){this.f.setAttribute(sf(a),b);tf(this)};
n.get=function(a){a=this.f.getAttribute(sf(a));if("string"!==typeof a&&null!==a)throw"Storage mechanism: Invalid value was encountered";return a};
n.remove=function(a){this.f.removeAttribute(sf(a));tf(this)};
n.J=function(a){var b=0,c=this.f.XMLDocument.documentElement.attributes,d=new Bd;d.next=function(){if(b>=c.length)throw Ad;var e=c[b++];if(a)return decodeURIComponent(e.nodeName.replace(/\./g,"%")).substr(1);e=e.nodeValue;if("string"!==typeof e)throw"Storage mechanism: Invalid value was encountered";return e};
return d};
n.clear=function(){for(var a=this.f.XMLDocument.documentElement,b=a.attributes.length;0<b;b--)a.removeAttribute(a.attributes[b-1].nodeName);tf(this)};
function tf(a){try{a.f.save(a.g)}catch(b){throw"Storage mechanism: Quota exceeded";}}
;function uf(a,b){this.g=a;this.f=b+"::"}
F(uf,mf);uf.prototype.set=function(a,b){this.g.set(this.f+a,b)};
uf.prototype.get=function(a){return this.g.get(this.f+a)};
uf.prototype.remove=function(a){this.g.remove(this.f+a)};
uf.prototype.J=function(a){var b=this.g.J(!0),c=this,d=new Bd;d.next=function(){for(var e=b.next();e.substr(0,c.f.length)!=c.f;)e=b.next();return a?e.substr(c.f.length):c.g.get(e)};
return d};function vf(a,b){1<b.length?a[b[0]]=b[1]:1===b.length&&Object.assign(a,b[0])}
;var wf=window.yt&&window.yt.config_||window.ytcfg&&window.ytcfg.data_||{};x("yt.config_",wf,void 0);function P(a){vf(wf,arguments)}
function Q(a,b){return a in wf?wf[a]:b}
function xf(){return Q("PLAYER_CONFIG",{})}
;var yf=[];function zf(a){yf.forEach(function(b){return b(a)})}
function Af(a){return a&&window.yterr?function(){try{return a.apply(this,arguments)}catch(b){Bf(b),zf(b)}}:a}
function Bf(a){var b=y("yt.logging.errors.log");b?b(a,"ERROR",void 0,void 0,void 0):(b=Q("ERRORS",[]),b.push([a,"ERROR",void 0,void 0,void 0]),P("ERRORS",b))}
function Cf(a){var b=y("yt.logging.errors.log");b?b(a,"WARNING",void 0,void 0,void 0):(b=Q("ERRORS",[]),b.push([a,"WARNING",void 0,void 0,void 0]),P("ERRORS",b))}
;var Df=window.yt&&window.yt.msgs_||window.ytcfg&&window.ytcfg.msgs||{};x("yt.msgs_",Df,void 0);function Ef(a){vf(Df,arguments)}
;function Ff(a,b,c,d){Cc.set(""+a,b,{va:c,path:"/",domain:void 0===d?"youtube.com":d,secure:!1})}
;function R(a){a=Gf(a);return"string"===typeof a&&"false"===a?!1:!!a}
function Hf(a,b){var c=Gf(a);return void 0===c&&void 0!==b?b:Number(c||0)}
function Gf(a){var b=Q("EXPERIMENTS_FORCED_FLAGS",{});return void 0!==b[a]?b[a]:Q("EXPERIMENT_FLAGS",{})[a]}
;function If(a){a&&(a.dataset?a.dataset[Jf("loaded")]="true":a.setAttribute("data-loaded","true"))}
function Kf(a,b){return a?a.dataset?a.dataset[Jf(b)]:a.getAttribute("data-"+b):null}
var Lf={};function Jf(a){return Lf[a]||(Lf[a]=String(a).replace(/\-([a-z])/g,function(b,c){return c.toUpperCase()}))}
;function S(a,b){"function"===typeof a&&(a=Af(a));return window.setTimeout(a,b)}
function U(a){window.clearTimeout(a)}
;var Mf=w.ytPubsubPubsubInstance||new O,Nf=w.ytPubsubPubsubSubscribedKeys||{},Of=w.ytPubsubPubsubTopicToKeys||{},Pf=w.ytPubsubPubsubIsSynchronous||{};function Qf(a,b){var c=Rf();if(c&&b){var d=c.subscribe(a,function(){var e=arguments;var f=function(){Nf[d]&&b.apply&&"function"==typeof b.apply&&b.apply(window,e)};
try{Pf[a]?f():S(f,0)}catch(g){Bf(g)}},void 0);
Nf[d]=!0;Of[a]||(Of[a]=[]);Of[a].push(d);return d}return 0}
function Sf(a){var b=Rf();b&&("number"===typeof a?a=[a]:"string"===typeof a&&(a=[parseInt(a,10)]),H(a,function(c){b.unsubscribeByKey(c);delete Nf[c]}))}
function Tf(a,b){var c=Rf();c&&c.publish.apply(c,arguments)}
function Uf(a){var b=Rf();if(b)if(b.clear(a),a)Vf(a);else for(var c in Of)Vf(c)}
function Rf(){return w.ytPubsubPubsubInstance}
function Vf(a){Of[a]&&(a=Of[a],H(a,function(b){Nf[b]&&delete Nf[b]}),a.length=0)}
O.prototype.subscribe=O.prototype.subscribe;O.prototype.unsubscribeByKey=O.prototype.T;O.prototype.publish=O.prototype.P;O.prototype.clear=O.prototype.clear;x("ytPubsubPubsubInstance",Mf,void 0);x("ytPubsubPubsubTopicToKeys",Of,void 0);x("ytPubsubPubsubIsSynchronous",Pf,void 0);x("ytPubsubPubsubSubscribedKeys",Nf,void 0);var Wf=/\.vflset|-vfl[a-zA-Z0-9_+=-]+/,Xf=/-[a-zA-Z]{2,3}_[a-zA-Z]{2,3}(?=(\/|$))/;function Yf(a,b,c){c=void 0===c?null:c;if(window.spf&&spf.script){c="";if(a){var d=a.indexOf("jsbin/"),e=a.lastIndexOf(".js"),f=d+6;-1<d&&-1<e&&e>f&&(c=a.substring(f,e),c=c.replace(Wf,""),c=c.replace(Xf,""),c=c.replace("debug-",""),c=c.replace("tracing-",""))}spf.script.load(a,c,b)}else Zf(a,b,c)}
function Zf(a,b,c){c=void 0===c?null:c;var d=$f(a),e=document.getElementById(d),f=e&&Kf(e,"loaded"),g=e&&!f;f?b&&b():(b&&(f=Qf(d,b),b=""+Na(b),ag[b]=f),g||(e=bg(a,d,function(){Kf(e,"loaded")||(If(e),Tf(d),S(Sa(Uf,d),0))},c)))}
function bg(a,b,c,d){d=void 0===d?null:d;var e=Kc(document,"SCRIPT");e.id=b;e.onload=function(){c&&setTimeout(c,0)};
e.onreadystatechange=function(){switch(e.readyState){case "loaded":case "complete":e.onload()}};
d&&e.setAttribute("nonce",d);Tb(e,Yc(a));a=document.getElementsByTagName("head")[0]||document.body;a.insertBefore(e,a.firstChild);return e}
function cg(a){a=$f(a);var b=document.getElementById(a);b&&(Uf(a),b.parentNode.removeChild(b))}
function dg(a,b){if(a&&b){var c=""+Na(b);(c=ag[c])&&Sf(c)}}
function $f(a){var b=document.createElement("a");Sb(b,a);a=b.href.replace(/^[a-zA-Z]+:\/\//,"//");return"js-"+Vb(a)}
var ag={};function eg(){}
function fg(a,b){return gg(a,1,b)}
;function hg(){}
v(hg,eg);function gg(a,b,c){isNaN(c)&&(c=void 0);var d=y("yt.scheduler.instance.addJob");return d?d(a,b,c):void 0===c?(a(),NaN):S(a,c||0)}
hg.prototype.start=function(){var a=y("yt.scheduler.instance.start");a&&a()};
hg.prototype.pause=function(){var a=y("yt.scheduler.instance.pause");a&&a()};
Ka(hg);hg.getInstance();var ig=[],jg=!1;function kg(){if(!R("disable_ad_status_on_html5_clients")&&(!R("condition_ad_status_fetch_on_consent_cookie_html5_clients")||Cc.get("CONSENT","").startsWith("YES+"))&&"1"!=gb(xf(),"args","privembed")){var a=function(){jg=!0;"google_ad_status"in window?P("DCLKSTAT",1):P("DCLKSTAT",2)};
try{Yf("//static.doubleclick.net/instream/ad_status.js",a)}catch(b){}ig.push(fg(function(){jg||"google_ad_status"in window||(dg("//static.doubleclick.net/instream/ad_status.js",a),jg=!0,P("DCLKSTAT",3))},5E3))}}
function lg(){return parseInt(Q("DCLKSTAT",0),10)}
;var mg=0;x("ytDomDomGetNextId",y("ytDomDomGetNextId")||function(){return++mg},void 0);var ng={stopImmediatePropagation:1,stopPropagation:1,preventMouseEvent:1,preventManipulation:1,preventDefault:1,layerX:1,layerY:1,screenX:1,screenY:1,scale:1,rotation:1,webkitMovementX:1,webkitMovementY:1};
function og(a){this.type="";this.state=this.source=this.data=this.currentTarget=this.relatedTarget=this.target=null;this.charCode=this.keyCode=0;this.metaKey=this.shiftKey=this.ctrlKey=this.altKey=!1;this.clientY=this.clientX=0;this.changedTouches=this.touches=null;try{if(a=a||window.event){this.event=a;for(var b in a)b in ng||(this[b]=a[b]);var c=a.target||a.srcElement;c&&3==c.nodeType&&(c=c.parentNode);this.target=c;var d=a.relatedTarget;if(d)try{d=d.nodeName?d:null}catch(e){d=null}else"mouseover"==
this.type?d=a.fromElement:"mouseout"==this.type&&(d=a.toElement);this.relatedTarget=d;this.clientX=void 0!=a.clientX?a.clientX:a.pageX;this.clientY=void 0!=a.clientY?a.clientY:a.pageY;this.keyCode=a.keyCode?a.keyCode:a.which;this.charCode=a.charCode||("keypress"==this.type?this.keyCode:0);this.altKey=a.altKey;this.ctrlKey=a.ctrlKey;this.shiftKey=a.shiftKey;this.metaKey=a.metaKey;this.f=a.pageX;this.g=a.pageY}}catch(e){}}
function pg(a){if(document.body&&document.documentElement){var b=document.body.scrollTop+document.documentElement.scrollTop;a.f=a.clientX+(document.body.scrollLeft+document.documentElement.scrollLeft);a.g=a.clientY+b}}
og.prototype.preventDefault=function(){this.event&&(this.event.returnValue=!1,this.event.preventDefault&&this.event.preventDefault())};
og.prototype.stopPropagation=function(){this.event&&(this.event.cancelBubble=!0,this.event.stopPropagation&&this.event.stopPropagation())};
og.prototype.stopImmediatePropagation=function(){this.event&&(this.event.cancelBubble=!0,this.event.stopImmediatePropagation&&this.event.stopImmediatePropagation())};var ib=w.ytEventsEventsListeners||{};x("ytEventsEventsListeners",ib,void 0);var qg=w.ytEventsEventsCounter||{count:0};x("ytEventsEventsCounter",qg,void 0);
function rg(a,b,c,d){d=void 0===d?{}:d;a.addEventListener&&("mouseenter"!=b||"onmouseenter"in document?"mouseleave"!=b||"onmouseenter"in document?"mousewheel"==b&&"MozBoxSizing"in document.documentElement.style&&(b="MozMousePixelScroll"):b="mouseout":b="mouseover");return hb(function(e){var f="boolean"===typeof e[4]&&e[4]==!!d,g=B(e[4])&&B(d)&&lb(e[4],d);return!!e.length&&e[0]==a&&e[1]==b&&e[2]==c&&(f||g)})}
var sg=Xa(function(){var a=!1;try{var b=Object.defineProperty({},"capture",{get:function(){a=!0}});
window.addEventListener("test",null,b)}catch(c){}return a});
function V(a,b,c,d){d=void 0===d?{}:d;if(!a||!a.addEventListener&&!a.attachEvent)return"";var e=rg(a,b,c,d);if(e)return e;e=++qg.count+"";var f=!("mouseenter"!=b&&"mouseleave"!=b||!a.addEventListener||"onmouseenter"in document);var g=f?function(h){h=new og(h);if(!Mc(h.relatedTarget,function(k){return k==a}))return h.currentTarget=a,h.type=b,c.call(a,h)}:function(h){h=new og(h);
h.currentTarget=a;return c.call(a,h)};
g=Af(g);a.addEventListener?("mouseenter"==b&&f?b="mouseover":"mouseleave"==b&&f?b="mouseout":"mousewheel"==b&&"MozBoxSizing"in document.documentElement.style&&(b="MozMousePixelScroll"),sg()||"boolean"===typeof d?a.addEventListener(b,g,d):a.addEventListener(b,g,!!d.capture)):a.attachEvent("on"+b,g);ib[e]=[a,b,c,g,d];return e}
function tg(a){a&&("string"==typeof a&&(a=[a]),H(a,function(b){if(b in ib){var c=ib[b],d=c[0],e=c[1],f=c[3];c=c[4];d.removeEventListener?sg()||"boolean"===typeof c?d.removeEventListener(e,f,c):d.removeEventListener(e,f,!!c.capture):d.detachEvent&&d.detachEvent("on"+e,f);delete ib[b]}}))}
;var ug=window.ytcsi&&window.ytcsi.now?window.ytcsi.now:window.performance&&window.performance.timing&&window.performance.now&&window.performance.timing.navigationStart?function(){return window.performance.timing.navigationStart+window.performance.now()}:function(){return(new Date).getTime()};function vg(a){this.u=a;this.f=null;this.j=0;this.m=null;this.l=0;this.h=[];for(a=0;4>a;a++)this.h.push(0);this.i=0;this.F=V(window,"mousemove",C(this.G,this));a=C(this.C,this);"function"===typeof a&&(a=Af(a));this.H=window.setInterval(a,25)}
F(vg,M);vg.prototype.G=function(a){void 0===a.f&&pg(a);var b=a.f;void 0===a.g&&pg(a);this.f=new Ec(b,a.g)};
vg.prototype.C=function(){if(this.f){var a=ug();if(0!=this.j){var b=this.m,c=this.f,d=b.x-c.x;b=b.y-c.y;d=Math.sqrt(d*d+b*b)/(a-this.j);this.h[this.i]=.5<Math.abs((d-this.l)/this.l)?1:0;for(c=b=0;4>c;c++)b+=this.h[c]||0;3<=b&&this.u();this.l=d}this.j=a;this.m=this.f;this.i=(this.i+1)%4}};
vg.prototype.o=function(){window.clearInterval(this.H);tg(this.F)};var wg={};
function xg(a){var b=void 0===a?{}:a;a=void 0===b.Na?!0:b.Na;b=void 0===b.Za?!1:b.Za;if(null==y("_lact",window)){var c=parseInt(Q("LACT"),10);c=isFinite(c)?E()-Math.max(c,0):-1;x("_lact",c,window);x("_fact",c,window);-1==c&&yg();V(document,"keydown",yg);V(document,"keyup",yg);V(document,"mousedown",yg);V(document,"mouseup",yg);a&&(b?V(window,"touchmove",function(){zg("touchmove",200)},{passive:!0}):(V(window,"resize",function(){zg("resize",200)}),V(window,"scroll",function(){zg("scroll",200)})));
new vg(function(){zg("mouse",100)});
V(document,"touchstart",yg,{passive:!0});V(document,"touchend",yg,{passive:!0})}}
function zg(a,b){wg[a]||(wg[a]=!0,fg(function(){yg();wg[a]=!1},b))}
function yg(){null==y("_lact",window)&&xg();var a=E();x("_lact",a,window);-1==y("_fact",window)&&x("_fact",a,window);(a=y("ytglobal.ytUtilActivityCallback_"))&&a()}
function Ag(){var a=y("_lact",window),b;null==a?b=-1:b=Math.max(E()-a,0);return b}
;var Bg=window,W=Bg.ytcsi&&Bg.ytcsi.now?Bg.ytcsi.now:Bg.performance&&Bg.performance.timing&&Bg.performance.now&&Bg.performance.timing.navigationStart?function(){return Bg.performance.timing.navigationStart+Bg.performance.now()}:function(){return(new Date).getTime()};var Cg=Hf("initial_gel_batch_timeout",1E3),Dg=Math.pow(2,16)-1,Eg=null,Fg=0,Gg=void 0,Hg=0,Ig=0,Jg=0,Kg=!0,Lg=w.ytLoggingTransportGELQueue_||new Map;x("ytLoggingTransportGELQueue_",Lg,void 0);var Mg=w.ytLoggingTransportTokensToCttTargetIds_||{};x("ytLoggingTransportTokensToCttTargetIds_",Mg,void 0);function Ng(){return new N(function(a){U(Hg);U(Ig);Ig=0;Gg&&Gg.isReady()?(Og(a),Lg.clear()):(Pg(),a())})}
function Pg(){R("web_gel_timeout_cap")&&!Ig&&(Ig=S(Ng,6E4));U(Hg);var a=Q("LOGGING_BATCH_TIMEOUT",Hf("web_gel_debounce_ms",1E4));R("shorten_initial_gel_batch_timeout")&&Kg&&(a=Cg);Hg=S(Ng,a)}
function Og(a){for(var b=Gg,c=Math.round(W()),d=Lg.size,e=t(Lg),f=e.next();!f.done;f=e.next()){var g=t(f.value);f=g.next().value;var h=g.next().value;g=nb({context:Qg(b.f||Rg())});g.events=h;(h=Mg[f])&&Sg(g,f,h);delete Mg[f];Tg(g,c);Ug(b,"log_event",g,{retry:!0,onSuccess:function(){d--;d||a();Fg=Math.round(W()-c)},
onError:function(){d--;d||a()}});
Kg=!1}}
function Tg(a,b){a.requestTimeMs=String(b);R("unsplit_gel_payloads_in_logs")&&(a.unsplitGelPayloadsInLogs=!0);var c=Q("EVENT_ID",void 0);if(c){var d=Q("BATCH_CLIENT_COUNTER",void 0)||0;!d&&R("web_client_counter_random_seed")&&(d=Math.floor(Math.random()*Dg/2));d++;d>Dg&&(d=1);P("BATCH_CLIENT_COUNTER",d);c={serializedEventId:c,clientCounter:String(d)};a.serializedClientEventId=c;Eg&&Fg&&R("log_gel_rtt_web")&&(a.previousBatchInfo={serializedClientEventId:Eg,roundtripMs:String(Fg)});Eg=c;Fg=0}}
function Sg(a,b,c){if(c.videoId)var d="VIDEO";else if(c.playlistId)d="PLAYLIST";else return;a.credentialTransferTokenTargetId=c;a.context=a.context||{};a.context.user=a.context.user||{};a.context.user.credentialTransferTokens=[{token:b,scope:d}]}
;var Vg=w.ytLoggingGelSequenceIdObj_||{};x("ytLoggingGelSequenceIdObj_",Vg,void 0);
function Wg(a,b,c,d){d=void 0===d?{}:d;var e={};e.eventTimeMs=Math.round(d.timestamp||W());e[a]=b;e.context={lastActivityMs:String(d.timestamp?-1:Ag())};R("log_sequence_info_on_gel_web")&&d.V&&(a=e.context,b=d.V,Vg[b]=b in Vg?Vg[b]+1:0,a.sequence={index:Vg[b],groupKey:b},d.Cb&&delete Vg[d.V]);d=d.ia;a="";d&&(a={},d.videoId?a.videoId=d.videoId:d.playlistId&&(a.playlistId=d.playlistId),Mg[d.token]=a,a=d.token);d=Lg.get(a)||[];Lg.set(a,d);d.push(e);c&&(Gg=new c);c=Hf("web_logging_max_batch")||100;e=
W();d.length>=c?Ng():10<=e-Jg&&(Pg(),Jg=e)}
;function Xg(){var a=Yg;y("yt.ads.biscotti.getId_")||x("yt.ads.biscotti.getId_",a,void 0)}
function Zg(a){x("yt.ads.biscotti.lastId_",a,void 0)}
;function $g(a){a=a.split("&");for(var b={},c=0,d=a.length;c<d;c++){var e=a[c].split("=");if(1==e.length&&e[0]||2==e.length)try{var f=decodeURIComponent((e[0]||"").replace(/\+/g," ")),g=decodeURIComponent((e[1]||"").replace(/\+/g," "));f in b?Array.isArray(b[f])?eb(b[f],g):b[f]=[b[f],g]:b[f]=g}catch(k){if("q"!=e[0]){var h=Error("Error decoding URL component");h.params={key:e[0],value:e[1]};Bf(h)}}}return b}
function ah(a){var b=[];fb(a,function(c,d){var e=encodeURIComponent(String(d)),f;Array.isArray(c)?f=c:f=[c];H(f,function(g){""==g?b.push(e):b.push(e+"="+encodeURIComponent(String(g)))})});
return b.join("&")}
function bh(a){"?"==a.charAt(0)&&(a=a.substr(1));return $g(a)}
function ch(a,b,c){var d=a.split("#",2);a=d[0];d=1<d.length?"#"+d[1]:"";var e=a.split("?",2);a=e[0];e=bh(e[1]||"");for(var f in b)!c&&null!==e&&f in e||(e[f]=b[f]);return ac(a,e)+d}
;function dh(a){var b=eh;a=void 0===a?y("yt.ads.biscotti.lastId_")||"":a;b=Object.assign(fh(b),gh(b));b.ca_type="image";a&&(b.bid=a);return b}
function fh(a){var b={};b.dt=ad;b.flash="0";a:{try{var c=a.f.top.location.href}catch(f){a=2;break a}a=c?c===a.g.location.href?0:1:2}b=(b.frm=a,b);b.u_tz=-(new Date).getTimezoneOffset();var d=void 0===d?L:d;try{var e=d.history.length}catch(f){e=0}b.u_his=e;b.u_java=!!L.navigator&&"unknown"!==typeof L.navigator.javaEnabled&&!!L.navigator.javaEnabled&&L.navigator.javaEnabled();L.screen&&(b.u_h=L.screen.height,b.u_w=L.screen.width,b.u_ah=L.screen.availHeight,b.u_aw=L.screen.availWidth,b.u_cd=L.screen.colorDepth);
L.navigator&&L.navigator.plugins&&(b.u_nplug=L.navigator.plugins.length);L.navigator&&L.navigator.mimeTypes&&(b.u_nmime=L.navigator.mimeTypes.length);return b}
function gh(a){var b=a.f;try{var c=b.screenX;var d=b.screenY}catch(p){}try{var e=b.outerWidth;var f=b.outerHeight}catch(p){}try{var g=b.innerWidth;var h=b.innerHeight}catch(p){}b=[b.screenLeft,b.screenTop,c,d,b.screen?b.screen.availWidth:void 0,b.screen?b.screen.availTop:void 0,e,f,g,h];c=a.f.top;try{var k=(c||window).document,l="CSS1Compat"==k.compatMode?k.documentElement:k.body;var m=(new Fc(l.clientWidth,l.clientHeight)).round()}catch(p){m=new Fc(-12245933,-12245933)}k=m;m={};l=new gd;w.SVGElement&&
w.document.createElementNS&&l.set(0);c=Rc();c["allow-top-navigation-by-user-activation"]&&l.set(1);c["allow-popups-to-escape-sandbox"]&&l.set(2);w.crypto&&w.crypto.subtle&&l.set(3);w.TextDecoder&&w.TextEncoder&&l.set(4);l=hd(l);m.bc=l;m.bih=k.height;m.biw=k.width;m.brdim=b.join();a=a.g;return m.vis={visible:1,hidden:2,prerender:3,preview:4,unloaded:5}[a.visibilityState||a.webkitVisibilityState||a.mozVisibilityState||""]||0,m.wgl=!!L.WebGLRenderingContext,m}
var eh=new function(){var a=window.document;this.f=window;this.g=a};
x("yt.ads_.signals_.getAdSignalsString",function(a){return ah(dh(a))},void 0);var hh="XMLHttpRequest"in w?function(){return new XMLHttpRequest}:null;
function ih(){if(!hh)return null;var a=hh();return"open"in a?a:null}
function jh(a){switch(a&&"status"in a?a.status:-1){case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 304:return!0;default:return!1}}
;var kh={Authorization:"AUTHORIZATION","X-Goog-Visitor-Id":"SANDBOXED_VISITOR_ID","X-YouTube-Client-Name":"INNERTUBE_CONTEXT_CLIENT_NAME","X-YouTube-Client-Version":"INNERTUBE_CONTEXT_CLIENT_VERSION","X-YouTube-Device":"DEVICE","X-Youtube-Identity-Token":"ID_TOKEN","X-YouTube-Page-CL":"PAGE_CL","X-YouTube-Page-Label":"PAGE_BUILD_LABEL","X-YouTube-Variants-Checksum":"VARIANTS_CHECKSUM"},lh="app debugcss debugjs expflag force_ad_params force_viral_ad_response_params forced_experiments innertube_snapshots innertube_goldens internalcountrycode internalipoverride absolute_experiments conditional_experiments sbb sr_bns_address client_dev_root_url".split(" "),
mh=!1;
function nh(a,b){b=void 0===b?{}:b;if(!c)var c=window.location.href;var d=a.match(Wb)[1]||null,e=K(a);d&&e?(d=c,c=a.match(Wb),d=d.match(Wb),c=c[3]==d[3]&&c[1]==d[1]&&c[4]==d[4]):c=e?K(c)==e&&(Number(c.match(Wb)[4]||null)||null)==(Number(a.match(Wb)[4]||null)||null):!0;d=R("web_ajax_ignore_global_headers_if_set");for(var f in kh)e=Q(kh[f]),!e||!c&&K(a)||d&&void 0!==b[f]||(b[f]=e);if(c||!K(a))b["X-YouTube-Utc-Offset"]=String(-(new Date).getTimezoneOffset());(c||!K(a))&&(f="undefined"!=typeof Intl?(new Intl.DateTimeFormat).resolvedOptions().timeZone:
null)&&(b["X-YouTube-Time-Zone"]=f);if(c||!K(a))b["X-YouTube-Ad-Signals"]=ah(dh(void 0));return b}
function oh(a){var b=window.location.search,c=K(a),d=Xb(a.match(Wb)[5]||null);d=(c=c&&(c.endsWith("youtube.com")||c.endsWith("youtube-nocookie.com")))&&d&&d.startsWith("/api/");if(!c||d)return a;var e=bh(b),f={};H(lh,function(g){e[g]&&(f[g]=e[g])});
return ch(a,f||{},!1)}
function ph(a,b){if(window.fetch&&"XML"!=b.format){var c={method:b.method||"GET",credentials:"same-origin"};b.headers&&(c.headers=b.headers);a=qh(a,b);var d=rh(a,b);d&&(c.body=d);b.withCredentials&&(c.credentials="include");var e=!1,f;fetch(a,c).then(function(g){if(!e){e=!0;f&&U(f);var h=g.ok,k=function(l){l=l||{};var m=b.context||w;h?b.onSuccess&&b.onSuccess.call(m,l,g):b.onError&&b.onError.call(m,l,g);b.qa&&b.qa.call(m,l,g)};
"JSON"==(b.format||"JSON")&&(h||400<=g.status&&500>g.status)?g.json().then(k,function(){k(null)}):k(null)}});
b.za&&0<b.timeout&&(f=S(function(){e||(e=!0,U(f),b.za.call(b.context||w))},b.timeout))}else sh(a,b)}
function sh(a,b){var c=b.format||"JSON";a=qh(a,b);var d=rh(a,b),e=!1,f=th(a,function(k){if(!e){e=!0;h&&U(h);var l=jh(k),m=null,p=400<=k.status&&500>k.status,u=500<=k.status&&600>k.status;if(l||p||u)m=uh(a,c,k,b.Bb);if(l)a:if(k&&204==k.status)l=!0;else{switch(c){case "XML":l=0==parseInt(m&&m.return_code,10);break a;case "RAW":l=!0;break a}l=!!m}m=m||{};p=b.context||w;l?b.onSuccess&&b.onSuccess.call(p,k,m):b.onError&&b.onError.call(p,k,m);b.qa&&b.qa.call(p,k,m)}},b.method,d,b.headers,b.responseType,
b.withCredentials);
if(b.U&&0<b.timeout){var g=b.U;var h=S(function(){e||(e=!0,f.abort(),U(h),g.call(b.context||w,f))},b.timeout)}return f}
function qh(a,b){b.Eb&&(a=document.location.protocol+"//"+document.location.hostname+(document.location.port?":"+document.location.port:"")+a);var c=Q("XSRF_FIELD_NAME",void 0),d=b.lb;d&&(d[c]&&delete d[c],a=ch(a,d||{},!0));return a}
function rh(a,b){var c=Q("XSRF_FIELD_NAME",void 0),d=Q("XSRF_TOKEN",void 0),e=b.postBody||"",f=b.D,g=Q("XSRF_FIELD_NAME",void 0),h;b.headers&&(h=b.headers["Content-Type"]);b.Db||K(a)&&!b.withCredentials&&K(a)!=document.location.hostname||"POST"!=b.method||h&&"application/x-www-form-urlencoded"!=h||b.D&&b.D[g]||(f||(f={}),f[c]=d);f&&"string"===typeof e&&(e=bh(e),pb(e,f),e=b.Aa&&"JSON"==b.Aa?JSON.stringify(e):Zb(e));f=e||f&&!jb(f);!mh&&f&&"POST"!=b.method&&(mh=!0,Bf(Error("AJAX request with postData should use POST")));
return e}
function uh(a,b,c,d){var e=null;switch(b){case "JSON":try{var f=c.responseText}catch(g){throw d=Error("Error reading responseText"),d.params=a,Cf(d),g;}a=c.getResponseHeader("Content-Type")||"";f&&0<=a.indexOf("json")&&(")]}'\n"===f.substring(0,5)&&(f=f.substring(5)),e=JSON.parse(f));break;case "XML":if(a=(a=c.responseXML)?vh(a):null)e={},H(a.getElementsByTagName("*"),function(g){e[g.tagName]=wh(g)})}d&&xh(e);
return e}
function xh(a){if(B(a))for(var b in a){var c;(c="html_content"==b)||(c=b.length-5,c=0<=c&&b.indexOf("_html",c)==c);if(c){c=b;var d=Rb(a[b],null);a[c]=d}else xh(a[b])}}
function vh(a){return a?(a=("responseXML"in a?a.responseXML:a).getElementsByTagName("root"))&&0<a.length?a[0]:null:null}
function wh(a){var b="";H(a.childNodes,function(c){b+=c.nodeValue});
return b}
function th(a,b,c,d,e,f,g){function h(){4==(k&&"readyState"in k?k.readyState:0)&&b&&Af(b)(k)}
c=void 0===c?"GET":c;d=void 0===d?"":d;var k=ih();if(!k)return null;"onloadend"in k?k.addEventListener("loadend",h,!1):k.onreadystatechange=h;R("debug_forward_web_query_parameters")&&(a=oh(a));k.open(c,a,!0);f&&(k.responseType=f);g&&(k.withCredentials=!0);c="POST"==c&&(void 0===window.FormData||!(d instanceof FormData));if(e=nh(a,e))for(var l in e)k.setRequestHeader(l,e[l]),"content-type"==l.toLowerCase()&&(c=!1);c&&k.setRequestHeader("Content-Type","application/x-www-form-urlencoded");k.send(d);
return k}
;function yh(){for(var a={},b=t(Object.entries(bh(Q("DEVICE","")))),c=b.next();!c.done;c=b.next()){var d=t(c.value);c=d.next().value;d=d.next().value;"cbrand"===c?a.deviceMake=d:"cmodel"===c?a.deviceModel=d:"cbr"===c?a.browserName=d:"cbrver"===c?a.browserVersion=d:"cos"===c?a.osName=d:"cosver"===c?a.osVersion=d:"cplatform"===c&&(a.platform=d)}return a}
;function zh(){return"INNERTUBE_API_KEY"in wf&&"INNERTUBE_API_VERSION"in wf}
function Rg(){return{innertubeApiKey:Q("INNERTUBE_API_KEY",void 0),innertubeApiVersion:Q("INNERTUBE_API_VERSION",void 0),Oa:Q("INNERTUBE_CONTEXT_CLIENT_CONFIG_INFO"),Pa:Q("INNERTUBE_CONTEXT_CLIENT_NAME","WEB"),innertubeContextClientVersion:Q("INNERTUBE_CONTEXT_CLIENT_VERSION",void 0),Ra:Q("INNERTUBE_CONTEXT_HL",void 0),Qa:Q("INNERTUBE_CONTEXT_GL",void 0),Sa:Q("INNERTUBE_HOST_OVERRIDE",void 0)||"",Ua:!!Q("INNERTUBE_USE_THIRD_PARTY_AUTH",!1),Ta:!!Q("INNERTUBE_OMIT_API_KEY_WHEN_AUTH_HEADER_IS_PRESENT",
!1),appInstallData:Q("SERIALIZED_CLIENT_CONFIG_DATA",void 0)}}
function Qg(a){var b={client:{hl:a.Ra,gl:a.Qa,clientName:a.Pa,clientVersion:a.innertubeContextClientVersion,configInfo:a.Oa}},c=window.devicePixelRatio;c&&1!=c&&(b.client.screenDensityFloat=String(c));c=Q("EXPERIMENTS_TOKEN","");""!==c&&(b.client.experimentsToken=c);c=[];var d=Q("EXPERIMENTS_FORCED_FLAGS",{});for(e in d)c.push({key:e,value:String(d[e])});var e=Q("EXPERIMENT_FLAGS",{});for(var f in e)f.startsWith("force_")&&void 0===d[f]&&c.push({key:f,value:String(e[f])});0<c.length&&(b.request={internalExperimentFlags:c});
a.appInstallData&&R("web_log_app_install_experiments")&&(b.client.configInfo=b.client.configInfo||{},b.client.configInfo.appInstallData=a.appInstallData);Q("DELEGATED_SESSION_ID")&&!R("pageid_as_header_web")&&(b.user={onBehalfOfUser:Q("DELEGATED_SESSION_ID")});b.client=Object.assign(b.client,yh());return b}
function Ah(a,b,c){c=void 0===c?{}:c;var d={"X-Goog-Visitor-Id":c.visitorData||Q("VISITOR_DATA","")};if(b&&b.includes("www.youtube-nocookie.com"))return d;(b=c.yb||Q("AUTHORIZATION"))||(a?b="Bearer "+y("gapi.auth.getToken")().xb:b=fd([]));b&&(d.Authorization=b,d["X-Goog-AuthUser"]=Q("SESSION_INDEX",0),R("pageid_as_header_web")&&(d["X-Goog-PageId"]=Q("DELEGATED_SESSION_ID")));return d}
;function Bh(a){a=Object.assign({},a);delete a.Authorization;var b=fd();if(b){var c=new yd;c.update(Q("INNERTUBE_API_KEY",void 0));c.update(b);b=c.digest();c=3;Ma(b);void 0===c&&(c=0);if(!wc){wc={};for(var d="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789".split(""),e=["+/=","+/","-_=","-_.","-_"],f=0;5>f;f++){var g=d.concat(e[f].split(""));vc[f]=g;for(var h=0;h<g.length;h++){var k=g[h];void 0===wc[k]&&(wc[k]=h)}}}c=vc[c];d=[];for(e=0;e<b.length;e+=3){var l=b[e],m=(f=e+1<b.length)?
b[e+1]:0;k=(g=e+2<b.length)?b[e+2]:0;h=l>>2;l=(l&3)<<4|m>>4;m=(m&15)<<2|k>>6;k&=63;g||(k=64,f||(m=64));d.push(c[h],c[l],c[m]||"",c[k]||"")}a.hash=d.join("")}return a}
;function Ch(){var a=new of;(a=a.isAvailable()?new uf(a,"yt.innertube"):null)||(a=new pf("yt.innertube"),a=a.isAvailable()?a:null);this.f=a?new kf(a):null;this.g=document.domain||window.location.hostname}
Ch.prototype.set=function(a,b,c,d){c=c||31104E3;this.remove(a);if(this.f)try{this.f.set(a,b,E()+1E3*c);return}catch(f){}var e="";if(d)try{e=escape(Ce(b))}catch(f){return}else e=escape(b);Ff(a,e,c,this.g)};
Ch.prototype.get=function(a,b){var c=void 0,d=!this.f;if(!d)try{c=this.f.get(a)}catch(e){d=!0}if(d&&(c=Cc.get(""+a,void 0))&&(c=unescape(c),b))try{c=JSON.parse(c)}catch(e){this.remove(a),c=void 0}return c};
Ch.prototype.remove=function(a){this.f&&this.f.remove(a);var b=this.g;Cc.remove(""+a,"/",void 0===b?"youtube.com":b)};var Dh;function Eh(){Dh||(Dh=new Ch);return Dh}
function Fh(a,b,c,d){if(d)return null;d=Eh().get("nextId",!0)||1;var e=Eh().get("requests",!0)||{};e[d]={method:a,request:b,authState:Bh(c),requestTime:Math.round(W())};Eh().set("nextId",d+1,86400,!0);Eh().set("requests",e,86400,!0);return d}
function Gh(a){var b=Eh().get("requests",!0)||{};delete b[a];Eh().set("requests",b,86400,!0)}
function Hh(a){var b=Eh().get("requests",!0);if(b){for(var c in b){var d=b[c];if(!(6E4>Math.round(W())-d.requestTime)){var e=d.authState,f=Bh(Ah(!1));lb(e,f)&&(e=d.request,"requestTimeMs"in e&&(e.requestTimeMs=Math.round(W())),Ug(a,d.method,e,{}));delete b[c]}}Eh().set("requests",b,86400,!0)}}
;new Ae;var Ih=tc||uc;function Jh(){var a=/WebKit\/([0-9]+)/.exec(Mb);return!!(a&&600<=parseInt(a[1],10))}
function Kh(){var a=/WebKit\/([0-9]+)/.exec(Mb);return!!(a&&602<=parseInt(a[1],10))}
function Lh(a){var b=Mb;return b?0<=b.toLowerCase().indexOf(a):!1}
;function Mh(a){if(!a)throw Error();throw a;}
function Nh(a){return a}
function X(a){var b=this;this.g=a;this.state={status:"PENDING"};this.f=[];this.onRejected=[];this.g(function(c){if("PENDING"===b.state.status){b.state={status:"FULFILLED",value:c};c=t(b.f);for(var d=c.next();!d.done;d=c.next())d=d.value,d()}},function(c){if("PENDING"===b.state.status){b.state={status:"REJECTED",
reason:c};c=t(b.onRejected);for(var d=c.next();!d.done;d=c.next())d=d.value,d()}})}
X.all=function(a){return new X(function(b,c){var d=[],e=a.length;0===e&&b(d);for(var f={L:0};f.L<a.length;f={L:f.L},++f.L)Oh(X.resolve(a[f.L]).then(function(g){return function(h){d[g.L]=h;e--;0===e&&b(d)}}(f)),function(g){c(g)})})};
X.resolve=function(a){return new X(function(b,c){a instanceof X?a.then(b,c):b(a)})};
X.reject=function(a){return new X(function(b,c){c(a)})};
X.prototype.then=function(a,b){var c=this,d=null!==a&&void 0!==a?a:Nh,e=null!==b&&void 0!==b?b:Mh;return new X(function(f,g){"PENDING"===c.state.status?(c.f.push(function(){Ph(c,c,d,f,g)}),c.onRejected.push(function(){Qh(c,c,e,f,g)})):"FULFILLED"===c.state.status?Ph(c,c,d,f,g):"REJECTED"===c.state.status&&Qh(c,c,e,f,g)})};
function Oh(a,b){a.then(void 0,b)}
function Ph(a,b,c,d,e){try{if("FULFILLED"!==a.state.status)throw Error("calling handleResolve before the promise is fulfilled.");var f=c(a.state.value);f instanceof X?Rh(a,b,f,d,e):d(f)}catch(g){e(g)}}
function Qh(a,b,c,d,e){try{if("REJECTED"!==a.state.status)throw Error("calling handleReject before the promise is rejected.");var f=c(a.state.reason);f instanceof X?Rh(a,b,f,d,e):d(f)}catch(g){e(g)}}
function Rh(a,b,c,d,e){b===c?e(new TypeError("Circular promise chain detected.")):c.then(function(f){f instanceof X?Rh(a,b,f,d,e):d(f)},function(f){e(f)})}
;function Sh(a,b,c){function d(){c(a.error);f()}
function e(){b(a.result);f()}
function f(){try{a.removeEventListener("success",e),a.removeEventListener("error",d)}catch(g){}}
a.addEventListener("success",e);a.addEventListener("error",d)}
function Th(a){return new N(function(b,c){Sh(a,b,c)})}
function Uh(a){return new X(function(b,c){Sh(a,b,c)})}
;function Vh(a,b){return new X(function(c,d){function e(){var f=a?b(a):null;f?f.then(function(g){a=g;e()},d):c()}
e()})}
;function Wh(a,b){for(var c=[],d=1;d<arguments.length;++d)c[d-1]=arguments[d];d=Error.call(this,a);this.message=d.message;"stack"in d&&(this.stack=d.stack);this.args=[].concat(c instanceof Array?c:fa(t(c)))}
v(Wh,Error);var Xh=[];var Yh={},Zh=(Yh.AUTH_INVALID="No user identifier specified.",Yh.EXPLICIT_ABORT="Transaction was explicitly aborted.",Yh.IDB_NOT_SUPPORTED="IndexedDB is not supported.",Yh.MISSING_OBJECT_STORE="Object store not created.",Yh.UNKNOWN_ABORT="Transaction was aborted for unknown reasons.",Yh.QUOTA_EXCEEDED="The current transaction exceeded its quota limitations.",Yh.QUOTA_MAYBE_EXCEEDED="The current transaction may have failed because of exceeding quota limitations.",Yh);
function $h(a,b,c){b=void 0===b?{}:b;c=void 0===c?Zh[a]:c;Wh.call(this,c,Object.assign({name:"YtIdbKnownError",isSw:void 0===self.document,isIframe:self!==self.top,type:a},b));this.type=a;this.message=c;Object.setPrototypeOf(this,$h.prototype);Xh.push({type:"ERROR",payload:this});10<Xh.length&&Xh.shift()}
v($h,Wh);function ai(a,b){$h.call(this,"UNKNOWN_ABORT",{objectStoreNames:a.join(),dbName:b});Object.setPrototypeOf(this,ai.prototype)}
v(ai,$h);function bi(a,b){this.f=a;this.options=b}
n=bi.prototype;n.add=function(a,b,c){return ci(this,[a],"readwrite",function(d){return di(d,a).add(b,c)})};
n.clear=function(a){return ci(this,[a],"readwrite",function(b){return di(b,a).clear()})};
n.close=function(){var a;this.f.close();(null===(a=this.options)||void 0===a?0:a.closed)&&this.options.closed()};
n.count=function(a,b){return ci(this,[a],"readonly",function(c){return di(c,a).count(b)})};
n["delete"]=function(a,b){return ci(this,[a],"readwrite",function(c){return di(c,a)["delete"](b)})};
n.get=function(a,b){return ci(this,[a],"readwrite",function(c){return di(c,a).get(b)})};
function ci(a,b,c,d){a=a.f.transaction(b,void 0===c?"readonly":c);a=new ei(a);return fi(a,d)}
function gi(a){this.f=a}
n=gi.prototype;n.add=function(a,b){return Uh(this.f.add(a,b))};
n.clear=function(){return Uh(this.f.clear()).then(function(){})};
n.count=function(a){return Uh(this.f.count(a))};
function hi(a,b){return ii(a,{query:b},function(c){return c["delete"]().then(function(){return c["continue"]()})}).then(function(){})}
n["delete"]=function(a){return a instanceof IDBKeyRange?hi(this,a):Uh(this.f["delete"](a))};
n.get=function(a){return Uh(this.f.get(a))};
n.index=function(a){return new ji(this.f.index(a))};
n.getName=function(){return this.f.name};
function ii(a,b,c){a=a.f.openCursor(b.query,b.direction);return ki(a).then(function(d){return Vh(d,c)})}
function ei(a){var b=this;this.f=a;this.g=new Map;this.aborted=!1;this.done=new N(function(c,d){b.f.addEventListener("complete",function(){c()});
b.f.addEventListener("error",function(e){e.currentTarget===e.target&&d(b.f.error)});
b.f.addEventListener("abort",function(){var e=b.f.error;if(e){var f;"QuotaExceededError"===e.name?f=new $h("QUOTA_EXCEEDED"):"UnknownError"===e.name&&(f=new $h("QUOTA_MAYBE_EXCEEDED"));f&&(Xh.push({type:"EVENT",eventType:"QUOTA_EXCEEDED",payload:void 0}),10<Xh.length&&Xh.shift());d(e)}else if(!b.aborted){e=ai;f=b.f.objectStoreNames;for(var g=[],h=0;h<f.length;h++){var k=f.item(h);if(null===k)throw Error("Invariant: item in DOMStringList is null");g.push(k)}e=new e(g,b.f.db.name);d(e)}})})}
function fi(a,b){var c=new N(function(d,e){Oh(b(a).then(function(f){a.commit();d(f)}),e)});
return Re([c,a.done]).then(function(d){return t(d).next().value})}
ei.prototype.abort=function(){this.f.abort();this.aborted=!0;throw new $h("EXPLICIT_ABORT");};
ei.prototype.commit=function(){var a=this.f;a.commit&&!this.aborted&&a.commit()};
function di(a,b){var c=a.f.objectStore(b),d=a.g.get(c);d||(d=new gi(c),a.g.set(c,d));return d}
function ji(a){this.f=a}
ji.prototype.count=function(a){return Uh(this.f.count(a))};
ji.prototype.get=function(a){return Uh(this.f.get(a))};
ji.prototype.getKey=function(a){return Uh(this.f.getKey(a))};
function li(a,b){this.request=a;this.cursor=b}
function ki(a){return Uh(a).then(function(b){return null===b?null:new li(a,b)})}
n=li.prototype;n.advance=function(a){this.cursor.advance(a);return ki(this.request)};
n["continue"]=function(a){this.cursor["continue"](a);return ki(this.request)};
n["delete"]=function(){return Uh(this.cursor["delete"]()).then(function(){})};
n.getKey=function(){return this.cursor.key};
n.getValue=function(){return this.cursor.value};
n.update=function(a){return Uh(this.cursor.update(a))};function mi(a){function b(){k||(k=new bi(c.result,{closed:h}));return k}
var c=self.indexedDB.open("yt-idb-test-do-not-use",void 0),d=a.blocked,e=a.blocking,f=a.Hb,g=a.upgrade,h=a.closed,k;g&&c.addEventListener("upgradeneeded",function(l){if(null===l.newVersion)throw Error("Invariant: newVersion on IDbVersionChangeEvent is null");if(null===c.transaction)throw Error("Invariant: transaction on IDbOpenDbRequest is null");var m=b(),p=new ei(c.transaction);g(m,l.oldVersion,l.newVersion,p)});
d&&c.addEventListener("blocked",function(){d()});
return Th(c).then(function(l){e&&l.addEventListener("versionchange",function(){e(b())});
f&&l.addEventListener("close",function(){f()});
return b()})}
function ni(){var a=void 0===a?{}:a;return mi(a)}
;var oi,pi,qi=["getAll","getAllKeys","getKey","openKeyCursor"],ri=["getAll","getAllKeys","getKey","openKeyCursor"];function si(){return cc(this,function b(){var c,d;return Ba(b,function(e){switch(e.g){case 1:if(Ih&&Jh()&&!Kh()&&!R("ytidb_allow_on_ios_safari_v8_and_v9")||gc)return e["return"](!1);try{if(c=self,!(c.indexedDB&&c.IDBIndex&&c.IDBKeyRange&&c.IDBObjectStore))return e["return"](!1)}catch(f){return e["return"](!1)}sa(e);return ra(e,Te(ni(),function(){}));
case 5:if(d=e.A,!d)return e["return"](!1);case 3:ua(e);if(d)try{d.close()}catch(f){}va(e);break;case 2:return ta(e),e["return"](!1);case 4:return e["return"](!0)}})})}
function ti(){return void 0!==oi?Ne(oi):new N(function(a){si().then(function(b){oi=b;a(b)})})}
function ui(){return void 0!==pi?Ne(pi):ti().then(function(a){if(!a)return!1;var b=t(qi);for(a=b.next();!a.done;a=b.next())if(!IDBObjectStore.prototype[a.value])return!1;b=t(ri);for(a=b.next();!a.done;a=b.next())if(!IDBIndex.prototype[a.value])return!1;return IDBObjectStore.prototype.getKey?!0:!1}).then(function(a){return pi=a})}
;function vi(){Ae.call(this);this.h=wi();xi(this);yi(this)}
v(vi,Ae);function wi(){var a=window.navigator.onLine;return void 0===a?!0:a}
function yi(a){window.addEventListener("online",function(){a.h=!0;a.m&&a.m()})}
function xi(a){window.addEventListener("offline",function(){a.h=!1;a.l&&a.l()})}
;function zi(a,b){b=void 0===b?{}:b;Ai(a,b)}
function Ai(a,b){b=void 0===b?{}:b;Bi().then(function(){vi.f||(vi.f=new vi);vi.f.h!==wi()&&Bf(Error("NetworkStatusManager isOnline does not match window status"));sh(a,b)})}
function Bi(){return cc(this,function b(){return Ba(b,function(c){return R("networkless_logging")?(2===Hf("networkless_ytidb_version")&&ui().then(function(d){return d}),c["return"](ti())):c["return"](!1)})})}
;function Ci(a){var b=this;this.f=null;a?this.f=a:zh()&&(this.f=Rg());gg(function(){Hh(b)},0,5E3)}
Ci.prototype.isReady=function(){!this.f&&zh()&&(this.f=Rg());return!!this.f};
function Ug(a,b,c,d){!Q("VISITOR_DATA")&&"visitor_id"!==b&&.01>Math.random()&&Cf(new Wh("Missing VISITOR_DATA when sending innertube request.",b,c,d));if(!a.isReady()){var e=new Wh("innertube xhrclient not ready",b,c,d);Bf(e);e.sampleWeight=0;throw e;}var f={headers:{"Content-Type":"application/json"},method:"POST",D:c,Aa:"JSON",U:function(){d.U()},
za:d.U,onSuccess:function(p,u){if(d.onSuccess)d.onSuccess(u)},
ya:function(p){if(d.onSuccess)d.onSuccess(p)},
onError:function(p,u){if(d.onError)d.onError(u)},
Fb:function(p){if(d.onError)d.onError(p)},
timeout:d.timeout,withCredentials:!0},g="";(e=a.f.Sa)&&(g=e);var h=a.f.Ua||!1,k=Ah(h,g,d);Object.assign(f.headers,k);f.headers.Authorization&&!g&&(f.headers["x-origin"]=window.location.origin);e="/youtubei/"+a.f.innertubeApiVersion+"/"+b;var l={alt:"json"};a.f.Ta&&f.headers.Authorization||(l.key=a.f.innertubeApiKey);var m=ch(""+g+e,l||{},!0);Bi().then(function(p){if(d.retry&&R("retry_web_logging_batches")&&"www.youtube-nocookie.com"!=g){if(R("networkless_gel")&&!p||!R("networkless_gel"))var u=Fh(b,
c,k,h);if(u){var r=f.onSuccess,A=f.ya;f.onSuccess=function(D,T){Gh(u);r(D,T)};
c.ya=function(D,T){Gh(u);A(D,T)}}}try{R("use_fetch_for_op_xhr")?ph(m,f):R("networkless_gel")&&d.retry?(f.method="POST",zi(m,f)):(f.method="POST",f.D||(f.D={}),sh(m,f))}catch(D){if("InvalidAccessError"==D.name)u&&(Gh(u),u=0),Cf(Error("An extension is blocking network request."));
else throw D;}u&&gg(function(){Hh(a)},0,5E3)})}
;function Di(a,b,c){c=void 0===c?{}:c;var d=Ci;Q("ytLoggingEventsDefaultDisabled",!1)&&Ci==Ci&&(d=null);Wg(a,b,d,c)}
;var Ei=[{wa:function(a){return"Cannot read property '"+a.key+"'"},
ra:{TypeError:[{regexp:/Cannot read property '([^']+)' of (null|undefined)/,groups:["key","value"]},{regexp:/\u65e0\u6cd5\u83b7\u53d6\u672a\u5b9a\u4e49\u6216 (null|undefined) \u5f15\u7528\u7684\u5c5e\u6027\u201c([^\u201d]+)\u201d/,groups:["value","key"]},{regexp:/\uc815\uc758\ub418\uc9c0 \uc54a\uc74c \ub610\ub294 (null|undefined) \ucc38\uc870\uc778 '([^']+)' \uc18d\uc131\uc744 \uac00\uc838\uc62c \uc218 \uc5c6\uc2b5\ub2c8\ub2e4./,groups:["value","key"]},{regexp:/No se puede obtener la propiedad '([^']+)' de referencia nula o sin definir/,
groups:["key"]},{regexp:/Unable to get property '([^']+)' of (undefined or null) reference/,groups:["key","value"]}],Error:[{regexp:/(Permission denied) to access property "([^']+)"/,groups:["reason","key"]}]}},{wa:function(a){return"Cannot call '"+a.key+"'"},
ra:{TypeError:[{regexp:/(?:([^ ]+)?\.)?([^ ]+) is not a function/,groups:["base","key"]},{regexp:/([^ ]+) called on (null or undefined)/,groups:["key","value"]},{regexp:/Object (.*) has no method '([^ ]+)'/,groups:["base","key"]},{regexp:/Object doesn't support property or method '([^ ]+)'/,groups:["key"]},{regexp:/\u30aa\u30d6\u30b8\u30a7\u30af\u30c8\u306f '([^']+)' \u30d7\u30ed\u30d1\u30c6\u30a3\u307e\u305f\u306f\u30e1\u30bd\u30c3\u30c9\u3092\u30b5\u30dd\u30fc\u30c8\u3057\u3066\u3044\u307e\u305b\u3093/,
groups:["key"]},{regexp:/\uac1c\uccb4\uac00 '([^']+)' \uc18d\uc131\uc774\ub098 \uba54\uc11c\ub4dc\ub97c \uc9c0\uc6d0\ud558\uc9c0 \uc54a\uc2b5\ub2c8\ub2e4./,groups:["key"]}]}}];function Fi(){this.f=[];this.g=[]}
var Gi;var Hi=new O;var Ii=new Set,Ji=0,Ki=["PhantomJS","Googlebot","TO STOP THIS SECURITY SCAN go/scan"];function Li(a){Mi(a,"WARNING")}
function Mi(a,b,c,d,e,f){f=void 0===f?{}:f;f.name=c||Q("INNERTUBE_CONTEXT_CLIENT_NAME",1);f.version=d||Q("INNERTUBE_CONTEXT_CLIENT_VERSION",void 0);c=f||{};b=void 0===b?"ERROR":b;b=void 0===b?"ERROR":b;var g=void 0===g?!1:g;if(a&&(R("console_log_js_exceptions")&&(d=[],d.push("Name: "+a.name),d.push("Message: "+a.message),a.hasOwnProperty("params")&&d.push("Error Params: "+JSON.stringify(a.params)),d.push("File name: "+a.fileName),d.push("Stacktrace: "+a.stack),window.console.log(d.join("\n"),a)),
(window&&window.yterr||g)&&!(5<=Ji)&&0!==a.sampleWeight)){var h=xc(a);g=h.message||"Unknown Error";d=h.name||"UnknownError";e=h.lineNumber||"Not available";f=h.fileName||"Not available";h=h.stack||a.f||"Not available";if(a.hasOwnProperty("args")&&a.args&&a.args.length)for(var k=0,l=0;l<a.args.length;l++){var m=a.args[l],p="params."+l;k+=p.length;if(m)if(Array.isArray(m))for(var u=c,r=0;r<m.length&&!(m[r]&&(k+=Ni(r,m[r],p,u),500<k));r++);else if("object"===typeof m)for(u in u=void 0,r=c,m){if(m[u]&&
(k+=Ni(u,m[u],p,r),500<k))break}else c[p]=String(JSON.stringify(m)).substring(0,500),k+=c[p].length;else c[p]=String(JSON.stringify(m)).substring(0,500),k+=c[p].length;if(500<=k)break}else if(a.hasOwnProperty("params")&&a.params)if(m=a.params,"object"===typeof a.params)for(l in p=0,m){if(m[l]&&(k="params."+l,u=String(JSON.stringify(m[l])).substr(0,500),c[k]=u,p+=k.length+u.length,500<p))break}else c.params=String(JSON.stringify(m)).substr(0,500);navigator.vendor&&!c.hasOwnProperty("vendor")&&(c.vendor=
navigator.vendor);c={message:g,name:d,lineNumber:e,fileName:f,stack:h,params:c};a=Number(a.columnNumber);isNaN(a)||(c.lineNumber=c.lineNumber+":"+a);a=t(Ei);for(g=a.next();!g.done;g=a.next())if(g=g.value,g.ra[c.name])for(e=t(g.ra[c.name]),d=e.next();!d.done;d=e.next())if(f=d.value,d=c.message.match(f.regexp)){c.params["error.original"]=d[0];e=f.groups;f={};for(h=0;h<e.length;h++)f[e[h]]=d[h+1],c.params["error."+e[h]]=d[h+1];c.message=g.wa(f);break}window.yterr&&"function"===typeof window.yterr&&window.yterr(c);
if(!(Ii.has(c.message)||0<=c.stack.indexOf("/YouTubeCenter.js")||0<=c.stack.indexOf("/mytube.js"))){Hi.P("handleError",c);if(R("kevlar_gel_error_routing")){a=b;a:{g=t(Ki);for(d=g.next();!d.done;d=g.next())if(Lh(d.value.toLowerCase())){g=!0;break a}g=!1}if(!g){g={stackTrace:c.stack};c.fileName&&(g.filename=c.fileName);d=c.lineNumber&&c.lineNumber.split?c.lineNumber.split(":"):[];0!==d.length&&(1!==d.length||isNaN(Number(d[0]))?2!==d.length||isNaN(Number(d[0]))||isNaN(Number(d[1]))||(g.lineNumber=Number(d[0]),
g.columnNumber=Number(d[1])):g.lineNumber=Number(d[0]));d=c.message;e=c.name;Gi||(Gi=new Fi);f=Gi;a:{h=t(f.g);for(l=h.next();!l.done;l=h.next())if(l=l.value,c.message&&c.message.match(l.f)){f=l.weight;break a}f=t(f.f);for(h=f.next();!h.done;h=f.next())if(h=h.value,h.Ga(c)){f=h.weight;break a}f=1}d={level:"ERROR_LEVEL_UNKNOWN",message:d,errorClassName:e,sampleWeight:f};"ERROR"===a?d.level="ERROR_LEVEL_ERROR":"WARNING"===a&&(d.level="ERROR_LEVEL_WARNNING");a={isObfuscated:!0,browserStackInfo:g};g={pageUrl:window.location.href};
Q("FEXP_EXPERIMENTS")&&(g.experimentIds=Q("FEXP_EXPERIMENTS"));g.kvPairs=[];if(e=c.params)for(f=t(Object.keys(e)),h=f.next();!h.done;h=f.next())h=h.value,g.kvPairs.push({key:"client."+h,value:String(e[h])});e=Q("SERVER_NAME",void 0);f=Q("SERVER_VERSION",void 0);e&&f&&(g.kvPairs.push({key:"server.name",value:e}),g.kvPairs.push({key:"server.version",value:f}));Di("clientError",{errorMetadata:g,stackTrace:a,logMessage:d});Ng()}}if(!R("suppress_error_204_logging")){a=c.params||{};b={lb:{a:"logerror",
t:"jserror",type:c.name,msg:c.message.substr(0,250),line:c.lineNumber,level:b,"client.name":a.name},D:{url:Q("PAGE_NAME",window.location.href),file:c.fileName},method:"POST"};a.version&&(b["client.version"]=a.version);if(b.D){c.stack&&(b.D.stack=c.stack);g=t(Object.keys(a));for(d=g.next();!d.done;d=g.next())d=d.value,b.D["client."+d]=a[d];if(a=Q("LATEST_ECATCHER_SERVICE_TRACKING_PARAMS",void 0))for(g=t(Object.keys(a)),d=g.next();!d.done;d=g.next())d=d.value,b.D[d]=a[d];a=Q("SERVER_NAME",void 0);g=
Q("SERVER_VERSION",void 0);a&&g&&(b.D["server.name"]=a,b.D["server.version"]=g)}sh(Q("ECATCHER_REPORT_HOST","")+"/error_204",b)}Ii.add(c.message);Ji++}}}
function Ni(a,b,c,d){c+="."+a;a=String(JSON.stringify(b)).substr(0,500);d[c]=a;return c.length+a.length}
;function Oi(){this.g=!1;this.f=null}
Oi.prototype.initialize=function(a,b,c,d,e,f){var g=this;f=void 0===f?!1:f;b?(this.g=!0,Yf(b,function(){g.g=!1;window.botguard?Pi(g,c,d,f):(cg(b),Li(new Wh("Unable to load Botguard","from "+b)))},e)):a&&(e=Kc(document,"SCRIPT"),e.textContent=a,e.nonce=Ga(),document.head.appendChild(e),document.head.removeChild(e),window.botguard?Pi(this,c,d,f):Li(Error("Unable to load Botguard from JS")))};
function Pi(a,b,c,d){if(d)try{a.f=new window.botguard.bg(b,c?function(){return c(b)}:z)}catch(e){Li(e)}else{try{a.f=new window.botguard.bg(b)}catch(e){Li(e)}c&&c(b)}}
Oi.prototype.dispose=function(){this.f=null};var Qi=new Oi;function Ri(){return!!Qi.f}
function Si(a){a=void 0===a?{}:a;a=void 0===a?{}:a;return Qi.f?Qi.f.invoke(void 0,void 0,a):null}
;var Ti=E().toString();
function Ui(){a:{if(window.crypto&&window.crypto.getRandomValues)try{var a=Array(16),b=new Uint8Array(16);window.crypto.getRandomValues(b);for(var c=0;c<a.length;c++)a[c]=b[c];var d=a;break a}catch(e){}d=Array(16);for(a=0;16>a;a++){b=E();for(c=0;c<b%23;c++)d[a]=Math.random();d[a]=Math.floor(256*Math.random())}if(Ti)for(a=1,b=0;b<Ti.length;b++)d[a%16]=d[a%16]^d[(a-1)%16]/4^Ti.charCodeAt(b),a++}a=[];for(b=0;b<d.length;b++)a.push("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_".charAt(d[b]&63));
return a.join("")}
;var Vi=w.ytLoggingDocDocumentNonce_||Ui();x("ytLoggingDocDocumentNonce_",Vi,void 0);var Wi=1;function Xi(a){this.f=a}
Xi.prototype.getAsJson=function(){var a={};void 0!==this.f.trackingParams?a.trackingParams=this.f.trackingParams:(a.veType=this.f.veType,void 0!==this.f.veCounter&&(a.veCounter=this.f.veCounter),void 0!==this.f.elementIndex&&(a.elementIndex=this.f.elementIndex));void 0!==this.f.dataElement&&(a.dataElement=this.f.dataElement.getAsJson());void 0!==this.f.youtubeData&&(a.youtubeData=this.f.youtubeData);return a};
Xi.prototype.toString=function(){return JSON.stringify(this.getAsJson())};
Xi.prototype.isClientVe=function(){return!this.f.trackingParams&&!!this.f.veType};function Yi(a){a=void 0===a?0:a;return 0==a?"client-screen-nonce":"client-screen-nonce."+a}
function Zi(a){a=void 0===a?0:a;return 0==a?"ROOT_VE_TYPE":"ROOT_VE_TYPE."+a}
function $i(a){return Q(Zi(void 0===a?0:a),void 0)}
x("yt_logging_screen.getRootVeType",$i,void 0);function aj(){var a=$i(0),b;a?b=new Xi({veType:a,youtubeData:void 0}):b=null;return b}
function bj(){var a=Q("csn-to-ctt-auth-info");a||(a={},P("csn-to-ctt-auth-info",a));return a}
function cj(a){a=void 0===a?0:a;var b=Q(Yi(a));if(!b&&!Q("USE_CSN_FALLBACK",!0))return null;b||0!=a||(R("kevlar_client_side_screens")||R("c3_client_side_screens")?b="UNDEFINED_CSN":b=Q("EVENT_ID"));return b?b:null}
x("yt_logging_screen.getCurrentCsn",cj,void 0);function dj(a,b,c){var d=bj();(c=cj(c))&&delete d[c];b&&(d[a]=b)}
function ej(a){return bj()[a]}
x("yt_logging_screen.getCttAuthInfo",ej,void 0);function fj(a,b,c,d){c=void 0===c?0:c;if(a!==Q(Yi(c))||b!==Q(Zi(c)))if(dj(a,d,c),P(Yi(c),a),P(Zi(c),b),0==c||R("web_screen_associated_all_layers"))b=function(){setTimeout(function(){a&&Wg("foregroundHeartbeatScreenAssociated",{clientDocumentNonce:Vi,clientScreenNonce:a},Ci)},0)},"requestAnimationFrame"in window?window.requestAnimationFrame(b):b()}
x("yt_logging_screen.setCurrentScreen",fj,void 0);function gj(a,b,c){b=void 0===b?{}:b;c=void 0===c?!1:c;var d=Q("EVENT_ID");d&&(b.ei||(b.ei=d));if(b){d=a;var e=void 0===e?!0:e;var f=Q("VALID_SESSION_TEMPDATA_DOMAINS",[]),g=K(window.location.href);g&&f.push(g);g=K(d);if(0<=Ya(f,g)||!g&&0==d.lastIndexOf("/",0))if(R("autoescape_tempdata_url")&&(f=document.createElement("a"),Sb(f,d),d=f.href),d){g=d.match(Wb);d=g[5];f=g[6];g=g[7];var h="";d&&(h+=d);f&&(h+="?"+f);g&&(h+="#"+g);d=h;f=d.indexOf("#");if(d=0>f?d:d.substr(0,f))if(e&&!b.csn&&(b.itct||b.ved)&&
(b=Object.assign({csn:cj()},b)),k){var k=parseInt(k,10);isFinite(k)&&0<k&&(e=b,b="ST-"+Vb(d).toString(36),e=e?Zb(e):"",Ff(b,e,k||5))}else k=b,e="ST-"+Vb(d).toString(36),k=k?Zb(k):"",Ff(e,k,5)}}if(c)return!1;if((window.ytspf||{}).enabled)spf.navigate(a);else{var l=void 0===l?{}:l;var m=void 0===m?"":m;var p=void 0===p?window:p;c=p.location;a=ac(a,l)+m;a=a instanceof I?a:Kb(a);c.href=Gb(a)}return!0}
;function hj(a,b){this.version=a;this.args=b}
;function ij(a,b){this.topic=a;this.f=b}
ij.prototype.toString=function(){return this.topic};var jj=y("ytPubsub2Pubsub2Instance")||new O;O.prototype.subscribe=O.prototype.subscribe;O.prototype.unsubscribeByKey=O.prototype.T;O.prototype.publish=O.prototype.P;O.prototype.clear=O.prototype.clear;x("ytPubsub2Pubsub2Instance",jj,void 0);var kj=y("ytPubsub2Pubsub2SubscribedKeys")||{};x("ytPubsub2Pubsub2SubscribedKeys",kj,void 0);var lj=y("ytPubsub2Pubsub2TopicToKeys")||{};x("ytPubsub2Pubsub2TopicToKeys",lj,void 0);var mj=y("ytPubsub2Pubsub2IsAsync")||{};x("ytPubsub2Pubsub2IsAsync",mj,void 0);
x("ytPubsub2Pubsub2SkipSubKey",null,void 0);function nj(a,b){var c=oj();c&&c.publish.call(c,a.toString(),a,b)}
function pj(a){var b=qj,c=oj();if(!c)return 0;var d=c.subscribe(b.toString(),function(e,f){var g=y("ytPubsub2Pubsub2SkipSubKey");g&&g==d||(g=function(){if(kj[d])try{if(f&&b instanceof ij&&b!=e)try{var h=b.f,k=f;if(!k.args||!k.version)throw Error("yt.pubsub2.Data.deserialize(): serializedData is incomplete.");try{if(!h.K){var l=new h;h.K=l.version}var m=h.K}catch(p){}if(!m||k.version!=m)throw Error("yt.pubsub2.Data.deserialize(): serializedData version is incompatible.");try{f=Reflect.construct(h,
db(k.args))}catch(p){throw p.message="yt.pubsub2.Data.deserialize(): "+p.message,p;}}catch(p){throw p.message="yt.pubsub2.pubsub2 cross-binary conversion error for "+b.toString()+": "+p.message,p;}a.call(window,f)}catch(p){Bf(p)}},mj[b.toString()]?y("yt.scheduler.instance")?fg(g):S(g,0):g())});
kj[d]=!0;lj[b.toString()]||(lj[b.toString()]=[]);lj[b.toString()].push(d);return d}
function rj(){var a=sj,b=pj(function(c){a.apply(void 0,arguments);tj(b)});
return b}
function tj(a){var b=oj();b&&("number"===typeof a&&(a=[a]),H(a,function(c){b.unsubscribeByKey(c);delete kj[c]}))}
function oj(){return y("ytPubsub2Pubsub2Instance")}
;function uj(a){hj.call(this,1,arguments);this.csn=a}
v(uj,hj);var qj=new ij("screen-created",uj),vj=[],wj=0;function xj(a,b,c){var d=R("use_default_events_client")?void 0:Ci;b={csn:a,parentVe:b.getAsJson(),childVes:$a(c,function(f){return f.getAsJson()})};
c=t(c);for(var e=c.next();!e.done;e=c.next())e=e.value.getAsJson(),(jb(e)||!e.trackingParams&&!e.veType)&&Li(Error("Child VE logged with no data"));c={ia:ej(a),V:a};"UNDEFINED_CSN"==a?yj("visualElementAttached",b,c):d?Wg("visualElementAttached",b,d,c):Di("visualElementAttached",b,c)}
function yj(a,b,c){vj.push({payloadName:a,payload:b,options:c});wj||(wj=rj())}
function sj(a){if(vj){for(var b=t(vj),c=b.next();!c.done;c=b.next())c=c.value,c.payload&&(c.payload.csn=a.csn,Wg(c.payloadName,c.payload,null,c.options));vj.length=0}wj=0}
;function zj(a){a=a||{};var b={},c={};this.url=a.url||"";this.args=a.args||mb(b);this.assets=a.assets||{};this.attrs=a.attrs||mb(c);this.fallback=a.fallback||null;this.fallbackMessage=a.fallbackMessage||null;this.html5=!!a.html5;this.disable=a.disable||{};this.loaded=!!a.loaded;this.messages=a.messages||{}}
zj.prototype.clone=function(){var a=new zj,b;for(b in this)if(this.hasOwnProperty(b)){var c=this[b];"object"==La(c)?a[b]=mb(c):a[b]=c}return a};function Aj(){M.call(this);this.f=[]}
v(Aj,M);Aj.prototype.o=function(){for(;this.f.length;){var a=this.f.pop();a.target.removeEventListener(a.name,a.Ga)}M.prototype.o.call(this)};var Bj=/cssbin\/(?:debug-)?([a-zA-Z0-9_-]+?)(?:-2x|-web|-rtl|-vfl|.css)/;function Cj(a){a=a||"";if(window.spf){var b=a.match(Bj);spf.style.load(a,b?b[1]:"",void 0)}else Dj(a)}
function Dj(a){var b=Ej(a),c=document.getElementById(b),d=c&&Kf(c,"loaded");d||c&&!d||(c=Fj(a,b,function(){Kf(c,"loaded")||(If(c),Tf(b),S(Sa(Uf,b),0))}))}
function Fj(a,b,c){var d=document.createElement("link");d.id=b;d.onload=function(){c&&setTimeout(c,0)};
a=Yc(a);d.rel="stylesheet";d.href=ub(a).toString();(document.getElementsByTagName("head")[0]||document.body).appendChild(d);return d}
function Ej(a){var b=Kc(document,"A");Sb(b,new I(a,Fb));a=b.href.replace(/^[a-zA-Z]+:\/\//,"//");return"css-"+Vb(a)}
;function Gj(a,b,c,d){M.call(this);var e=this;this.m=this.aa=a;this.G=b;this.u=!1;this.api={};this.Y=this.F=null;this.H=new O;Sc(this,Sa(Tc,this.H));this.j={};this.N=this.Z=this.i=this.ha=this.f=null;this.M=!1;this.l=this.C=null;this.ba={};this.Da=["onReady"];this.ga=null;this.sa=NaN;this.W={};this.h=d;Hj(this);this.ca("WATCH_LATER_VIDEO_ADDED",this.Wa.bind(this));this.ca("WATCH_LATER_VIDEO_REMOVED",this.Xa.bind(this));this.ca("onAdAnnounce",this.Fa.bind(this));this.Ea=new Aj(this);Sc(this,Sa(Tc,this.Ea));
this.X=0;c?this.X=S(function(){e.loadNewVideoConfig(c)},0):d&&(Ij(this),Jj(this))}
v(Gj,M);n=Gj.prototype;n.getId=function(){return this.G};
n.loadNewVideoConfig=function(a){if(!this.g){this.X&&(U(this.X),this.X=0);a instanceof zj||(a=new zj(a));this.ha=a;this.f=a.clone();Ij(this);this.Z||(this.Z=Kj(this,this.f.args.jsapicallback||"onYouTubePlayerReady"));this.f.args.jsapicallback=null;if(a=this.f.attrs.width)this.m.style.width=$c(Number(a)||String(a));if(a=this.f.attrs.height)this.m.style.height=$c(Number(a)||String(a));Jj(this);this.u&&Lj(this)}};
function Ij(a){var b;a.h?b=a.h.rootElementId:b=a.f.attrs.id;a.i=b||a.i;"video-player"==a.i&&(a.i=a.G,a.h?a.h.rootElementId=a.G:a.f.attrs.id=a.G);a.m.id==a.i&&(a.i+="-player",a.h?a.h.rootElementId=a.i:a.f.attrs.id=a.i)}
n.Ka=function(){return this.ha};
function Lj(a){a.f&&!a.f.loaded&&(a.f.loaded=!0,"0"!=a.f.args.autoplay?a.api.loadVideoByPlayerVars(a.f.args):a.api.cueVideoByPlayerVars(a.f.args))}
function Mj(a){var b=!0,c=Nj(a);c&&a.f&&(a=Oj(a),b=Kf(c,"version")===a);return b&&!!y("yt.player.Application.create")}
function Jj(a){if(!a.g&&!a.M){var b=Mj(a);if(b&&"html5"==(Nj(a)?"html5":null))a.N="html5",a.u||Pj(a);else if(Qj(a),a.N="html5",b&&a.l)a.aa.appendChild(a.l),Pj(a);else{a.f&&(a.f.loaded=!0);var c=!1;a.C=function(){c=!0;var d=Rj(a,"player_bootstrap_method")?y("yt.player.Application.createAlternate")||y("yt.player.Application.create"):y("yt.player.Application.create");var e=a.f?a.f.clone():void 0;d(a.aa,e,a.h);Pj(a)};
a.M=!0;b?a.C():(Yf(Oj(a),a.C),(b=a.h?a.h.cssUrl:a.f.assets.css)&&Cj(b),Sj(a)&&!c&&x("yt.player.Application.create",null,void 0))}}}
function Nj(a){var b=Gc(a.i);!b&&a.m&&a.m.querySelector&&(b=a.m.querySelector("#"+a.i));return b}
function Pj(a){if(!a.g){var b=Nj(a),c=!1;b&&b.getApiInterface&&b.getApiInterface()&&(c=!0);c?(a.M=!1,!Rj(a,"html5_remove_not_servable_check_killswitch")&&b.isNotServable&&a.f&&b.isNotServable(a.f.args.video_id)||Tj(a)):a.sa=S(function(){Pj(a)},50)}}
function Tj(a){Hj(a);a.u=!0;var b=Nj(a);b.addEventListener&&(a.F=Uj(a,b,"addEventListener"));b.removeEventListener&&(a.Y=Uj(a,b,"removeEventListener"));var c=b.getApiInterface();c=c.concat(b.getInternalApiInterface());for(var d=0;d<c.length;d++){var e=c[d];a.api[e]||(a.api[e]=Uj(a,b,e))}for(var f in a.j)a.F(f,a.j[f]);Lj(a);a.Z&&a.Z(a.api);a.H.P("onReady",a.api)}
function Uj(a,b,c){var d=b[c];return function(){try{return a.ga=null,d.apply(b,arguments)}catch(e){"sendAbandonmentPing"!=c&&(e.params=c,a.ga=e,Cf(e))}}}
function Hj(a){a.u=!1;if(a.Y)for(var b in a.j)a.Y(b,a.j[b]);for(var c in a.W)U(parseInt(c,10));a.W={};a.F=null;a.Y=null;for(var d in a.api)a.api[d]=null;a.api.addEventListener=a.ca.bind(a);a.api.removeEventListener=a.bb.bind(a);a.api.destroy=a.dispose.bind(a);a.api.getLastError=a.La.bind(a);a.api.getPlayerType=a.Ma.bind(a);a.api.getCurrentVideoConfig=a.Ka.bind(a);a.api.loadNewVideoConfig=a.loadNewVideoConfig.bind(a);a.api.isReady=a.Va.bind(a)}
n.Va=function(){return this.u};
n.ca=function(a,b){var c=this,d=Kj(this,b);if(d){if(!(0<=Ya(this.Da,a)||this.j[a])){var e=Vj(this,a);this.F&&this.F(a,e)}this.H.subscribe(a,d);"onReady"==a&&this.u&&S(function(){d(c.api)},0)}};
n.bb=function(a,b){if(!this.g){var c=Kj(this,b);c&&df(this.H,a,c)}};
function Kj(a,b){var c=b;if("string"==typeof b){if(a.ba[b])return a.ba[b];c=function(){var d=y(b);d&&d.apply(w,arguments)};
a.ba[b]=c}return c?c:null}
function Vj(a,b){var c="ytPlayer"+b+a.G;a.j[b]=c;w[c]=function(d){var e=S(function(){if(!a.g){a.H.P(b,d);var f=a.W,g=String(e);g in f&&delete f[g]}},0);
kb(a.W,String(e))};
return c}
n.Fa=function(a){Tf("a11y-announce",a)};
n.Wa=function(a){Tf("WATCH_LATER_VIDEO_ADDED",a)};
n.Xa=function(a){Tf("WATCH_LATER_VIDEO_REMOVED",a)};
n.Ma=function(){return this.N||(Nj(this)?"html5":null)};
n.La=function(){return this.ga};
function Qj(a){a.cancel();Hj(a);a.N=null;a.f&&(a.f.loaded=!1);var b=Nj(a);b&&(Mj(a)||!Sj(a)?a.l=b:(b&&b.destroy&&b.destroy(),a.l=null));for(a=a.aa;b=a.firstChild;)a.removeChild(b)}
n.cancel=function(){if(this.C){var a=Oj(this);dg(a,this.C)}U(this.sa);this.M=!1};
n.o=function(){Qj(this);if(this.l&&this.f&&this.l.destroy)try{this.l.destroy()}catch(b){Bf(b)}this.ba=null;for(var a in this.j)w[this.j[a]]=null;this.ha=this.f=this.api=null;delete this.aa;delete this.m;M.prototype.o.call(this)};
function Sj(a){return a.f&&a.f.args&&a.f.args.fflags?-1!=a.f.args.fflags.indexOf("player_destroy_old_version=true"):!1}
function Oj(a){return a.h?a.h.jsUrl:a.f.assets.js}
function Rj(a,b){if(a.h)var c=a.h.serializedExperimentFlags;else a.f&&a.f.args&&(c=a.f.args.fflags);return"true"==$g(c||"")[b]}
;var Wj={},Xj="player_uid_"+(1E9*Math.random()>>>0);
function Yj(a,b,c){var d="player";c=void 0===c?!0:c;var e;"string"===typeof d?e=Gc(d):e=d;d=e;e=Xj+"_"+Na(d);var f=Wj[e];if(f&&c)return(b&&b.serializedExperimentFlags?b.serializedExperimentFlags.includes("web_player_remove_playerproxy=true"):a&&a.args&&a.args.fflags&&a.args.fflags.includes("web_player_remove_playerproxy=true"))?f.api.loadVideoByPlayerVars(a.args||null):f.loadNewVideoConfig(a),f.api;f=new Gj(d,e,a,b);Wj[e]=f;Tf("player-added",f.api);Sc(f,Sa(Zj,f));return f.api}
function Zj(a){delete Wj[a.getId()]}
;function ak(a){return(0===a.search("cue")||0===a.search("load"))&&"loadModule"!==a}
function bk(a,b,c){"string"===typeof a&&(a={mediaContentUrl:a,startSeconds:b,suggestedQuality:c});a:{if((b=a.mediaContentUrl)&&(b=/\/([ve]|embed)\/([^#?]+)/.exec(b))&&b[2]){b=b[2];break a}b=null}a.videoId=b;return ck(a)}
function ck(a,b,c){if("string"===typeof a)return{videoId:a,startSeconds:b,suggestedQuality:c};b=["endSeconds","startSeconds","mediaContentUrl","suggestedQuality","videoId"];c={};for(var d=0;d<b.length;d++){var e=b[d];a[e]&&(c[e]=a[e])}return c}
function dk(a,b,c,d){if(B(a)&&!Array.isArray(a)){b="playlist list listType index startSeconds suggestedQuality".split(" ");c={};for(d=0;d<b.length;d++){var e=b[d];a[e]&&(c[e]=a[e])}return c}b={index:b,startSeconds:c,suggestedQuality:d};"string"===typeof a&&16===a.length?b.list="PL"+a:b.playlist=a;return b}
;function ek(a){a=void 0===a?!1:a;M.call(this);this.f=new O(a);Sc(this,Sa(Tc,this.f))}
F(ek,M);ek.prototype.subscribe=function(a,b,c){return this.g?0:this.f.subscribe(a,b,c)};
ek.prototype.j=function(a,b){this.g||this.f.P.apply(this.f,arguments)};function fk(a,b,c){ek.call(this);this.h=a;this.i=b;this.l=c}
v(fk,ek);function gk(a,b,c){if(!a.g){var d=a.h;d.g||a.i!=d.f||(a={id:a.l,command:b},c&&(a.data=c),d.f.postMessage(Ce(a),d.i))}}
fk.prototype.o=function(){this.i=this.h=null;ek.prototype.o.call(this)};function hk(a){M.call(this);this.f=a;this.f.subscribe("command",this.Ba,this);this.h={};this.j=!1}
v(hk,M);n=hk.prototype;n.start=function(){this.j||this.g||(this.j=!0,gk(this.f,"RECEIVING"))};
n.Ba=function(a,b,c){if(this.j&&!this.g){var d=b||{};switch(a){case "addEventListener":"string"===typeof d.event&&(a=d.event,a in this.h||(c=C(this.eb,this,a),this.h[a]=c,this.addEventListener(a,c)));break;case "removeEventListener":"string"===typeof d.event&&ik(this,d.event);break;default:this.i.isReady()&&this.i.isExternalMethodAvailable(a,c||null)&&(b=jk(a,b||{}),c=this.i.handleExternalCall(a,b,c||null),(c=kk(a,c))&&this.j&&!this.g&&gk(this.f,a,c))}}};
n.eb=function(a,b){this.j&&!this.g&&gk(this.f,a,this.ka(a,b))};
n.ka=function(a,b){if(null!=b)return{value:b}};
function ik(a,b){b in a.h&&(a.removeEventListener(b,a.h[b]),delete a.h[b])}
n.o=function(){var a=this.f;a.g||df(a.f,"command",this.Ba,this);this.f=null;for(var b in this.h)ik(this,b);M.prototype.o.call(this)};function lk(a,b){hk.call(this,b);this.i=a;this.start()}
v(lk,hk);lk.prototype.addEventListener=function(a,b){this.i.addEventListener(a,b)};
lk.prototype.removeEventListener=function(a,b){this.i.removeEventListener(a,b)};
function jk(a,b){switch(a){case "loadVideoById":return b=ck(b),[b];case "cueVideoById":return b=ck(b),[b];case "loadVideoByPlayerVars":return[b];case "cueVideoByPlayerVars":return[b];case "loadPlaylist":return b=dk(b),[b];case "cuePlaylist":return b=dk(b),[b];case "seekTo":return[b.seconds,b.allowSeekAhead];case "playVideoAt":return[b.index];case "setVolume":return[b.volume];case "setPlaybackQuality":return[b.suggestedQuality];case "setPlaybackRate":return[b.suggestedRate];case "setLoop":return[b.loopPlaylists];
case "setShuffle":return[b.shufflePlaylist];case "getOptions":return[b.module];case "getOption":return[b.module,b.option];case "setOption":return[b.module,b.option,b.value];case "handleGlobalKeyDown":return[b.keyCode,b.shiftKey,b.ctrlKey,b.altKey,b.metaKey,b.key,b.code]}return[]}
function kk(a,b){switch(a){case "isMuted":return{muted:b};case "getVolume":return{volume:b};case "getPlaybackRate":return{playbackRate:b};case "getAvailablePlaybackRates":return{availablePlaybackRates:b};case "getVideoLoadedFraction":return{videoLoadedFraction:b};case "getPlayerState":return{playerState:b};case "getCurrentTime":return{currentTime:b};case "getPlaybackQuality":return{playbackQuality:b};case "getAvailableQualityLevels":return{availableQualityLevels:b};case "getDuration":return{duration:b};
case "getVideoUrl":return{videoUrl:b};case "getVideoEmbedCode":return{videoEmbedCode:b};case "getPlaylist":return{playlist:b};case "getPlaylistIndex":return{playlistIndex:b};case "getOptions":return{options:b};case "getOption":return{option:b}}}
lk.prototype.ka=function(a,b){switch(a){case "onReady":return;case "onStateChange":return{playerState:b};case "onPlaybackQualityChange":return{playbackQuality:b};case "onPlaybackRateChange":return{playbackRate:b};case "onError":return{errorCode:b}}return hk.prototype.ka.call(this,a,b)};
lk.prototype.o=function(){hk.prototype.o.call(this);delete this.i};function mk(a,b,c){M.call(this);var d=this;c=c||Q("POST_MESSAGE_ORIGIN",void 0)||window.document.location.protocol+"//"+window.document.location.hostname;this.h=b||null;this.u="*";this.i=c;this.sessionId=null;this.channel="widget";this.C=!!a;this.m=function(e){a:if(!("*"!=d.i&&e.origin!=d.i||d.h&&e.source!=d.h||"string"!==typeof e.data)){try{var f=JSON.parse(e.data)}catch(g){break a}if(!(null==f||d.C&&(d.sessionId&&d.sessionId!=f.id||d.channel&&d.channel!=f.channel))&&f)switch(f.event){case "listening":"null"!=
e.origin&&(d.i=d.u=e.origin);d.h=e.source;d.sessionId=f.id;d.f&&(d.f(),d.f=null);break;case "command":d.j&&(!d.l||0<=Ya(d.l,f.func))&&d.j(f.func,f.args,e.origin)}}};
this.l=this.f=this.j=null;window.addEventListener("message",this.m)}
v(mk,M);mk.prototype.sendMessage=function(a,b){var c=b||this.h;if(c){this.sessionId&&(a.id=this.sessionId);this.channel&&(a.channel=this.channel);try{var d=JSON.stringify(a);c.postMessage(d,this.u)}catch(e){Cf(e)}}};
mk.prototype.o=function(){window.removeEventListener("message",this.m);M.prototype.o.call(this)};function nk(){var a=this.g=new mk(!!Q("WIDGET_ID_ENFORCE")),b=C(this.ab,this);a.j=b;a.l=null;this.g.channel="widget";if(a=Q("WIDGET_ID"))this.g.sessionId=a;this.i=[];this.l=!1;this.j={}}
n=nk.prototype;n.ab=function(a,b,c){"addEventListener"==a&&b?(a=b[0],this.j[a]||"onReady"==a||(this.addEventListener(a,ok(this,a)),this.j[a]=!0)):this.xa(a,b,c)};
n.xa=function(){};
function ok(a,b){return C(function(c){this.sendMessage(b,c)},a)}
n.addEventListener=function(){};
n.Ja=function(){this.l=!0;this.sendMessage("initialDelivery",this.la());this.sendMessage("onReady");H(this.i,this.Ca,this);this.i=[]};
n.la=function(){return null};
function pk(a,b){a.sendMessage("infoDelivery",b)}
n.Ca=function(a){this.l?this.g.sendMessage(a):this.i.push(a)};
n.sendMessage=function(a,b){this.Ca({event:a,info:void 0==b?null:b})};
n.dispose=function(){this.g=null};function qk(a){nk.call(this);this.f=a;this.h=[];this.addEventListener("onReady",C(this.Ya,this));this.addEventListener("onVideoProgress",C(this.ib,this));this.addEventListener("onVolumeChange",C(this.jb,this));this.addEventListener("onApiChange",C(this.cb,this));this.addEventListener("onPlaybackQualityChange",C(this.fb,this));this.addEventListener("onPlaybackRateChange",C(this.gb,this));this.addEventListener("onStateChange",C(this.hb,this));this.addEventListener("onWebglSettingsChanged",C(this.kb,
this))}
v(qk,nk);n=qk.prototype;n.xa=function(a,b,c){if(this.f.isExternalMethodAvailable(a,c)){b=b||[];if(0<b.length&&ak(a)){var d=b;if(B(d[0])&&!Array.isArray(d[0]))d=d[0];else{var e={};switch(a){case "loadVideoById":case "cueVideoById":e=ck.apply(window,d);break;case "loadVideoByUrl":case "cueVideoByUrl":e=bk.apply(window,d);break;case "loadPlaylist":case "cuePlaylist":e=dk.apply(window,d)}d=e}b.length=1;b[0]=d}this.f.handleExternalCall(a,b,c);ak(a)&&pk(this,this.la())}};
n.Ya=function(){var a=C(this.Ja,this);this.g.f=a};
n.addEventListener=function(a,b){this.h.push({eventType:a,listener:b});this.f.addEventListener(a,b)};
n.la=function(){if(!this.f)return null;var a=this.f.getApiInterface();cb(a,"getVideoData");for(var b={apiInterface:a},c=0,d=a.length;c<d;c++){var e=a[c];if(0===e.search("get")||0===e.search("is")){var f=0;0===e.search("get")?f=3:0===e.search("is")&&(f=2);f=e.charAt(f).toLowerCase()+e.substr(f+1);try{var g=this.f[e]();b[f]=g}catch(h){}}}b.videoData=this.f.getVideoData();b.currentTimeLastUpdated_=E()/1E3;return b};
n.hb=function(a){a={playerState:a,currentTime:this.f.getCurrentTime(),duration:this.f.getDuration(),videoData:this.f.getVideoData(),videoStartBytes:0,videoBytesTotal:this.f.getVideoBytesTotal(),videoLoadedFraction:this.f.getVideoLoadedFraction(),playbackQuality:this.f.getPlaybackQuality(),availableQualityLevels:this.f.getAvailableQualityLevels(),currentTimeLastUpdated_:E()/1E3,playbackRate:this.f.getPlaybackRate(),mediaReferenceTime:this.f.getMediaReferenceTime()};this.f.getVideoUrl&&(a.videoUrl=
this.f.getVideoUrl());this.f.getVideoContentRect&&(a.videoContentRect=this.f.getVideoContentRect());this.f.getProgressState&&(a.progressState=this.f.getProgressState());this.f.getPlaylist&&(a.playlist=this.f.getPlaylist());this.f.getPlaylistIndex&&(a.playlistIndex=this.f.getPlaylistIndex());this.f.getStoryboardFormat&&(a.storyboardFormat=this.f.getStoryboardFormat());pk(this,a)};
n.fb=function(a){pk(this,{playbackQuality:a})};
n.gb=function(a){pk(this,{playbackRate:a})};
n.cb=function(){for(var a=this.f.getOptions(),b={namespaces:a},c=0,d=a.length;c<d;c++){var e=a[c],f=this.f.getOptions(e);b[e]={options:f};for(var g=0,h=f.length;g<h;g++){var k=f[g],l=this.f.getOption(e,k);b[e][k]=l}}this.sendMessage("apiInfoDelivery",b)};
n.jb=function(){pk(this,{muted:this.f.isMuted(),volume:this.f.getVolume()})};
n.ib=function(a){a={currentTime:a,videoBytesLoaded:this.f.getVideoBytesLoaded(),videoLoadedFraction:this.f.getVideoLoadedFraction(),currentTimeLastUpdated_:E()/1E3,playbackRate:this.f.getPlaybackRate(),mediaReferenceTime:this.f.getMediaReferenceTime()};this.f.getProgressState&&(a.progressState=this.f.getProgressState());pk(this,a)};
n.kb=function(){var a={sphericalProperties:this.f.getSphericalProperties()};pk(this,a)};
n.dispose=function(){nk.prototype.dispose.call(this);for(var a=0;a<this.h.length;a++){var b=this.h[a];this.f.removeEventListener(b.eventType,b.listener)}this.h=[]};function rk(a,b,c){M.call(this);this.f=a;this.i=c;this.j=V(window,"message",C(this.l,this));this.h=new fk(this,a,b);Sc(this,Sa(Tc,this.h))}
v(rk,M);rk.prototype.l=function(a){var b;if(b=!this.g)if(b=a.origin==this.i)a:{b=this.f;do{b:{var c=a.source;do{if(c==b){c=!0;break b}if(c==c.parent)break;c=c.parent}while(null!=c);c=!1}if(c){b=!0;break a}b=b.opener}while(null!=b);b=!1}if(b&&(b=a.data,"string"===typeof b)){try{b=JSON.parse(b)}catch(d){return}b.command&&(c=this.h,c.g||c.j("command",b.command,b.data,a.origin))}};
rk.prototype.o=function(){tg(this.j);this.f=null;M.prototype.o.call(this)};function sk(){var a=mb(tk),b;return Te(new N(function(c,d){a.onSuccess=function(e){jh(e)?c(e):d(new uk("Request failed, status="+(e&&"status"in e?e.status:-1),"net.badstatus",e))};
a.onError=function(e){d(new uk("Unknown request error","net.unknown",e))};
a.U=function(e){d(new uk("Request timed out","net.timeout",e))};
b=sh("//googleads.g.doubleclick.net/pagead/id",a)}),function(c){c instanceof Ue&&b.abort();
return Oe(c)})}
function uk(a,b,c){G.call(this,a+", errorCode="+b);this.errorCode=b;this.xhr=c;this.name="PromiseAjaxError"}
v(uk,G);function vk(){this.g=0;this.f=null}
vk.prototype.then=function(a,b,c){return 1===this.g&&a?(a=a.call(c,this.f),Ie(a)?a:wk(a)):2===this.g&&b?(a=b.call(c,this.f),Ie(a)?a:xk(a)):this};
vk.prototype.getValue=function(){return this.f};
vk.prototype.$goog_Thenable=!0;function xk(a){var b=new vk;a=void 0===a?null:a;b.g=2;b.f=void 0===a?null:a;return b}
function wk(a){var b=new vk;a=void 0===a?null:a;b.g=1;b.f=void 0===a?null:a;return b}
;function yk(a){G.call(this,a.message||a.description||a.name);this.isMissing=a instanceof zk;this.isTimeout=a instanceof uk&&"net.timeout"==a.errorCode;this.isCanceled=a instanceof Ue}
v(yk,G);yk.prototype.name="BiscottiError";function zk(){G.call(this,"Biscotti ID is missing from server")}
v(zk,G);zk.prototype.name="BiscottiMissingError";var tk={format:"RAW",method:"GET",timeout:5E3,withCredentials:!0},Ak=null;
function Yg(){if(R("disable_biscotti_fetch_on_html5_clients"))return Oe(Error("Fetching biscotti ID is disabled."));if(R("condition_biscotti_fetch_on_consent_cookie_html5_clients")&&!Cc.get("CONSENT","").startsWith("YES+"))return Oe(Error("User has not consented - not fetching biscotti id."));if("1"===gb(xf(),"args","privembed"))return Oe(Error("Biscotti ID is not available in private embed mode"));Ak||(Ak=Te(sk().then(Bk),function(a){return Ck(2,a)}));
return Ak}
function Bk(a){a=a.responseText;if(0!=a.lastIndexOf(")]}'",0))throw new zk;a=JSON.parse(a.substr(4));if(1<(a.type||1))throw new zk;a=a.id;Zg(a);Ak=wk(a);Dk(18E5,2);return a}
function Ck(a,b){var c=new yk(b);Zg("");Ak=xk(c);0<a&&Dk(12E4,a-1);throw c;}
function Dk(a,b){S(function(){Te(sk().then(Bk,function(c){return Ck(b,c)}),z)},a)}
function Ek(){try{var a=y("yt.ads.biscotti.getId_");return a?a():Yg()}catch(b){return Oe(b)}}
;function Fk(a){if("1"!==gb(xf(),"args","privembed")){a&&Xg();try{Ek().then(function(){},function(){}),S(Fk,18E5)}catch(b){Bf(b)}}}
;var Y=y("ytglobal.prefsUserPrefsPrefs_")||{};x("ytglobal.prefsUserPrefsPrefs_",Y,void 0);function Gk(){this.f=Q("ALT_PREF_COOKIE_NAME","PREF");var a=Cc.get(""+this.f,void 0);if(a){a=decodeURIComponent(a).split("&");for(var b=0;b<a.length;b++){var c=a[b].split("="),d=c[0];(c=c[1])&&(Y[d]=c.toString())}}}
n=Gk.prototype;n.get=function(a,b){Hk(a);Ik(a);var c=void 0!==Y[a]?Y[a].toString():null;return null!=c?c:b?b:""};
n.set=function(a,b){Hk(a);Ik(a);if(null==b)throw Error("ExpectedNotNull");Y[a]=b.toString()};
n.remove=function(a){Hk(a);Ik(a);delete Y[a]};
n.save=function(){Ff(this.f,this.dump(),63072E3)};
n.clear=function(){for(var a in Y)delete Y[a]};
n.dump=function(){var a=[],b;for(b in Y)a.push(b+"="+encodeURIComponent(String(Y[b])));return a.join("&")};
function Ik(a){if(/^f([1-9][0-9]*)$/.test(a))throw Error("ExpectedRegexMatch: "+a);}
function Hk(a){if(!/^\w+$/.test(a))throw Error("ExpectedRegexMismatch: "+a);}
function Jk(a){a=void 0!==Y[a]?Y[a].toString():null;return null!=a&&/^[A-Fa-f0-9]+$/.test(a)?parseInt(a,16):null}
Ka(Gk);function Kk(a,b){for(var c=[],d=1;d<arguments.length;++d)c[d-1]=arguments[d];if(!Lk(a)||c.some(function(e){return!Lk(e)}))throw Error("Only objects may be merged.");
c=t(c);for(d=c.next();!d.done;d=c.next())Mk(a,d.value);return a}
function Mk(a,b){for(var c in b)if(Lk(b[c])){if(c in a&&!Lk(a[c]))throw Error("Cannot merge an object into a non-object.");c in a||(a[c]={});Mk(a[c],b[c])}else if(Nk(b[c])){if(c in a&&!Nk(a[c]))throw Error("Cannot merge an array into a non-array.");c in a||(a[c]=[]);Ok(a[c],b[c])}else a[c]=b[c];return a}
function Ok(a,b){for(var c=t(b),d=c.next();!d.done;d=c.next())d=d.value,Lk(d)?a.push(Mk({},d)):Nk(d)?a.push(Ok([],d)):a.push(d);return a}
function Lk(a){return"object"===typeof a&&!Array.isArray(a)}
function Nk(a){return"object"===typeof a&&Array.isArray(a)}
;var Pk={},Qk=0;
function Rk(a,b,c,d,e){e=void 0===e?"":e;if(a)if(c&&!Lh("cobalt")){if(a){a instanceof I||(a="object"==typeof a&&a.R?a.O():String(a),Jb.test(a)?a=new I(a,Fb):(a=String(a),a=a.replace(/(%0A|%0D)/g,""),a=(b=a.match(Ib))&&Hb.test(b[1])?new I(a,Fb):null));a=Gb(a||Lb);if("about:invalid#zClosurez"===a||a.startsWith("data"))a="";else{if(!(a instanceof Pb)){b="object"==typeof a;var f=null;b&&a.ma&&(f=a.ja());a=Rb(wb(b&&a.R?a.O():String(a)),f)}a instanceof Pb&&a.constructor===Pb?a=a.f:(La(a),a="type_error:SafeHtml");
a=encodeURIComponent(String(Ce(a.toString())))}/^[\s\xa0]*$/.test(a)||(a=Jc("IFRAME",{src:'javascript:"<body><img src=\\""+'+a+'+"\\"></body>"',style:"display:none"}),(9==a.nodeType?a:a.ownerDocument||a.document).body.appendChild(a))}}else if(e)th(a,b,"POST",e,d);else if(Q("USE_NET_AJAX_FOR_PING_TRANSPORT",!1)||d)th(a,b,"GET","",d);else{b:{try{var g=new Wa({url:a});if(g.h&&g.g||g.i){var h=Xb(a.match(Wb)[5]||null),k;if(!(k=!h||!h.endsWith("/aclk"))){var l=a.search(bc);d:{for(c=0;0<=(c=a.indexOf("ri",
c))&&c<l;){var m=a.charCodeAt(c-1);if(38==m||63==m){var p=a.charCodeAt(c+2);if(!p||61==p||38==p||35==p){var u=c;break d}}c+=3}u=-1}if(0>u)var r=null;else{var A=a.indexOf("&",u);if(0>A||A>l)A=l;u+=3;r=decodeURIComponent(a.substr(u,A-u).replace(/\+/g," "))}k="1"!==r}f=!k;break b}}catch(D){}f=!1}f?Sk(a)?(b&&b(),f=!0):f=!1:f=!1;f||Tk(a,b)}}
function Sk(a,b){try{if(window.navigator&&window.navigator.sendBeacon&&window.navigator.sendBeacon(a,void 0===b?"":b))return!0}catch(c){}return!1}
function Tk(a,b){var c=new Image,d=""+Qk++;Pk[d]=c;c.onload=c.onerror=function(){b&&Pk[d]&&b();delete Pk[d]};
c.src=a}
;function Uk(a,b){hj.call(this,1,arguments)}
v(Uk,hj);function Vk(a,b){hj.call(this,1,arguments)}
v(Vk,hj);var Wk=new ij("aft-recorded",Uk),Xk=new ij("timing-sent",Vk);var Yk=window;function Zk(){this.timing={};this.clearResourceTimings=function(){};
this.webkitClearResourceTimings=function(){};
this.mozClearResourceTimings=function(){};
this.msClearResourceTimings=function(){};
this.oClearResourceTimings=function(){}}
var $k=Yk.performance||Yk.mozPerformance||Yk.msPerformance||Yk.webkitPerformance||new Zk;var al=!1;C($k.clearResourceTimings||$k.webkitClearResourceTimings||$k.mozClearResourceTimings||$k.msClearResourceTimings||$k.oClearResourceTimings||z,$k);function bl(a){var b=cl(a);if(b.aft)return b.aft;a=Q((a||"")+"TIMING_AFT_KEYS",["ol"]);for(var c=a.length,d=0;d<c;d++){var e=b[a[d]];if(e)return e}return NaN}
function dl(a){var b;(b=y("ytcsi."+(a||"")+"data_"))||(b={tick:{},info:{}},Ua("ytcsi."+(a||"")+"data_",b));return b}
function el(a){a=dl(a);a.info||(a.info={});return a.info}
function cl(a){a=dl(a);a.tick||(a.tick={});return a.tick}
function fl(a){var b=dl(a).nonce;b||(b=Ui(),dl(a).nonce=b);return b}
function gl(a){var b=cl(a||""),c=bl(a);c&&!al&&(nj(Wk,new Uk(Math.round(c-b._start),a)),al=!0)}
;function hl(){var a=y("ytcsi.debug");a||(a=[],x("ytcsi.debug",a,void 0),x("ytcsi.reference",{},void 0));return a}
function il(a){a=a||"";var b=y("ytcsi.reference");b||(hl(),b=y("ytcsi.reference"));if(b[a])return b[a];var c=hl(),d={timerName:a,info:{},tick:{},span:{}};c.push(d);return b[a]=d}
;var jl=w.ytLoggingLatencyUsageStats_||{};x("ytLoggingLatencyUsageStats_",jl,void 0);function kl(){this.f=0}
function ll(){kl.f||(kl.f=new kl);return kl.f}
kl.prototype.tick=function(a,b,c){ml(this,"tick_"+a+"_"+b)||Di("latencyActionTicked",{tickName:a,clientActionNonce:b},{timestamp:c})};
kl.prototype.info=function(a,b){var c=Object.keys(a).join("");ml(this,"info_"+c+"_"+b)||(c=Object.assign({},a),c.clientActionNonce=b,Di("latencyActionInfo",c))};
kl.prototype.span=function(a,b){var c=Object.keys(a).join("");ml(this,"span_"+c+"_"+b)||(a.clientActionNonce=b,Di("latencyActionSpan",a))};
function ml(a,b){jl[b]=jl[b]||{count:0};var c=jl[b];c.count++;c.time=W();a.f||(a.f=gg(function(){var d=W(),e;for(e in jl)jl[e]&&6E4<d-jl[e].time&&delete jl[e];a&&(a.f=0)},0,5E3));
return 5<c.count?(6===c.count&&1>1E5*Math.random()&&(c=new Wh("CSI data exceeded logging limit with key",b.split("_")),0<=b.indexOf("plev")||Li(c)),!0):!1}
;var Z={},nl=(Z.ad_allowed="adTypesAllowed",Z.yt_abt="adBreakType",Z.ad_cpn="adClientPlaybackNonce",Z.ad_docid="adVideoId",Z.yt_ad_an="adNetworks",Z.ad_at="adType",Z.aida="appInstallDataAgeMs",Z.browse_id="browseId",Z.p="httpProtocol",Z.t="transportProtocol",Z.cpn="clientPlaybackNonce",Z.ccs="creatorInfo.creatorCanaryState",Z.cseg="creatorInfo.creatorSegment",Z.csn="clientScreenNonce",Z.docid="videoId",Z.GetHome_rid="requestIds",Z.GetSearch_rid="requestIds",Z.GetPlayer_rid="requestIds",Z.GetWatchNext_rid=
"requestIds",Z.GetBrowse_rid="requestIds",Z.GetLibrary_rid="requestIds",Z.is_continuation="isContinuation",Z.is_nav="isNavigation",Z.b_p="kabukiInfo.browseParams",Z.is_prefetch="kabukiInfo.isPrefetch",Z.is_secondary_nav="kabukiInfo.isSecondaryNav",Z.prev_browse_id="kabukiInfo.prevBrowseId",Z.query_source="kabukiInfo.querySource",Z.voz_type="kabukiInfo.vozType",Z.yt_lt="loadType",Z.mver="creatorInfo.measurementVersion",Z.yt_ad="isMonetized",Z.nr="webInfo.navigationReason",Z.nrsu="navigationRequestedSameUrl",
Z.ncnp="webInfo.nonPreloadedNodeCount",Z.pnt="performanceNavigationTiming",Z.prt="playbackRequiresTap",Z.plt="playerInfo.playbackType",Z.pis="playerInfo.playerInitializedState",Z.paused="playerInfo.isPausedOnLoad",Z.yt_pt="playerType",Z.fmt="playerInfo.itag",Z.yt_pl="watchInfo.isPlaylist",Z.yt_pre="playerInfo.preloadType",Z.yt_ad_pr="prerollAllowed",Z.pa="previousAction",Z.yt_red="isRedSubscriber",Z.rce="mwebInfo.responseContentEncoding",Z.scrh="screenHeight",Z.scrw="screenWidth",Z.st="serverTimeMs",
Z.ssdm="shellStartupDurationMs",Z.aq="tvInfo.appQuality",Z.br_trs="tvInfo.bedrockTriggerState",Z.kebqat="kabukiInfo.earlyBrowseRequestInfo.abandonmentType",Z.kebqa="kabukiInfo.earlyBrowseRequestInfo.adopted",Z.label="tvInfo.label",Z.is_mdx="tvInfo.isMdx",Z.preloaded="tvInfo.isPreloaded",Z.upg_player_vis="playerInfo.visibilityState",Z.query="unpluggedInfo.query",Z.upg_chip_ids_string="unpluggedInfo.upgChipIdsString",Z.yt_vst="videoStreamType",Z.vph="viewportHeight",Z.vpw="viewportWidth",Z.yt_vis="isVisible",
Z.rcl="mwebInfo.responseContentLength",Z.GetSettings_rid="requestIds",Z.GetTrending_rid="requestIds",Z.GetMusicSearchSuggestions_rid="requestIds",Z.REQUEST_ID="requestIds",Z),ol="isContinuation isNavigation kabukiInfo.earlyBrowseRequestInfo.adopted kabukiInfo.isPrefetch kabukiInfo.isSecondaryNav isMonetized navigationRequestedSameUrl performanceNavigationTiming playerInfo.isPausedOnLoad prerollAllowed isRedSubscriber tvInfo.isMdx tvInfo.isPreloaded isVisible watchInfo.isPlaylist playbackRequiresTap".split(" "),
pl={},ql=(pl.ccs="CANARY_STATE_",pl.mver="MEASUREMENT_VERSION_",pl.pis="PLAYER_INITIALIZED_STATE_",pl.yt_pt="LATENCY_PLAYER_",pl.pa="LATENCY_ACTION_",pl.yt_vst="VIDEO_STREAM_TYPE_",pl),rl="all_vc ap c cver cbrand cmodel cplatform ctheme ei l_an l_mm plid srt yt_fss yt_li vpst vpni2 vpil2 icrc icrt pa GetAccountOverview_rid GetHistory_rid cmt d_vpct d_vpnfi d_vpni nsru pc pfa pfeh pftr pnc prerender psc rc start tcrt tcrc ssr vpr vps yt_abt yt_fn yt_fs yt_pft yt_pre yt_pt yt_pvis ytu_pvis yt_ref yt_sts tds".split(" ");
function sl(a){return!!Q("FORCE_CSI_ON_GEL",!1)||R("csi_on_gel")||!!dl(a).useGel}
function tl(a){a=dl(a);if(!("gel"in a))a.gel={gelTicks:{},gelInfos:{}};else if(a.gel){var b=a.gel;b.gelInfos||(b.gelInfos={});b.gelTicks||(b.gelTicks={})}return a.gel}
;function ul(a,b,c){if(null!==b)if(el(c)[a]=b,sl(c)){var d=b;b=tl(c);if(b.gelInfos)b.gelInfos["info_"+a]=!0;else{var e={};b.gelInfos=(e["info_"+a]=!0,e)}if(a.match("_rid")){var f=a.split("_rid")[0];a="REQUEST_ID"}if(a in nl){b=nl[a];0<=Ya(ol,b)&&(d=!!d);a in ql&&"string"===typeof d&&(d=ql[a]+d.toUpperCase());a=d;d=b.split(".");for(var g=e={},h=0;h<d.length-1;h++){var k=d[h];g[k]={};g=g[k]}g[d[d.length-1]]="requestIds"===b?[{id:a,endpoint:f}]:a;f=Kk({},e)}else 0<=Ya(rl,a)||Li(new Wh("Unknown label logged with GEL CSI",
a)),f=void 0;f&&sl(c)&&(b=il(c||""),Kk(b.info,f),b=tl(c),"gelInfos"in b||(b.gelInfos={}),Kk(b.gelInfos,f),c=fl(c),ll().info(f,c))}else il(c||"").info[a]=b}
function vl(a,b,c){var d=cl(c);if(R("use_first_tick")&&wl(a,c))return d[a];if(!b&&"_"!==a[0]){var e=a;$k.mark&&(0==e.lastIndexOf("mark_",0)||(e="mark_"+e),c&&(e+=" ("+c+")"),$k.mark(e))}e=b||W();d[a]=e;e=tl(c);e.gelTicks&&(e.gelTicks["tick_"+a]=!0);c||b||W();if(sl(c)){il(c||"").tick[a]=b||W();e=fl(c);if("_start"===a){var f=ll();ml(f,"baseline_"+e)||Di("latencyActionBaselined",{clientActionNonce:e},{timestamp:b})}else ll().tick(a,e,b);gl(c);e=!0}else e=!1;if(!e){if(!y("yt.timing."+(c||"")+"pingSent_")&&
(f=Q((c||"")+"TIMING_ACTION",void 0),e=cl(c),y("ytglobal.timing"+(c||"")+"ready_")&&f&&wl("_start")&&bl(c)))if(gl(c),c)xl(c);else{f=!0;var g=Q("TIMING_WAIT",[]);if(g.length)for(var h=0,k=g.length;h<k;++h)if(!(g[h]in e)){f=!1;break}f&&xl(c)}il(c||"").tick[a]=b||W()}return d[a]}
function wl(a,b){var c=cl(b);return a in c}
function xl(a){if(!sl(a)){var b=cl(a),c=el(a),d=b._start,e=Q("CSI_SERVICE_NAME","youtube"),f={v:2,s:e,action:Q((a||"")+"TIMING_ACTION",void 0)},g=c.srt;void 0!==b.srt&&delete c.srt;b.aft=bl(a);var h=cl(a),k=h.pbr,l=h.vc;h=h.pbs;k&&l&&h&&k<l&&l<h&&el(a).yt_pvis&&"youtube"===e&&(ul("yt_lt","hot_bg",a),e=b.vc,k=b.pbs,delete b.aft,c.aft=Math.round(k-e));for(var m in c)"_"!==m.charAt(0)&&(f[m]=c[m]);b.ps=W();m={};e=[];for(var p in b)"_"!==p.charAt(0)&&(k=Math.round(b[p]-d),m[p]=k,e.push(p+"."+k));f.rt=
e.join(",");b=!!c.ap;R("debug_csi_data")&&(c=y("yt.timing.csiData"),c||(c=[],Ua("yt.timing.csiData",c)),c.push({page:location.href,time:new Date,args:f}));c="";for(var u in f)f.hasOwnProperty(u)&&(c+="&"+u+"="+f[u]);f="/csi_204?"+c.substring(1);if(window.navigator&&window.navigator.sendBeacon&&b){var r=void 0===r?"":r;Sk(f,r)||Rk(f,void 0,void 0,void 0,r)}else Rk(f);x("yt.timing."+(a||"")+"pingSent_",!0,void 0);nj(Xk,new Vk(m.aft+(Number(g)||0),a))}}
var yl=window;yl.ytcsi&&(yl.ytcsi.info=ul,yl.ytcsi.tick=vl);var zl=null,Al=null,Bl=null,Cl={};function Dl(a){var b=a.id;a=a.ve_type;var c=Wi++;a=new Xi({veType:a,veCounter:c,elementIndex:void 0,dataElement:void 0,youtubeData:void 0});Cl[b]=a;b=cj();c=aj();b&&c&&xj(b,c,[a])}
function El(){var a=zl.getVideoData(1);a=a.title?a.title+" - YouTube":"YouTube";document.title!==a&&(document.title=a)}
function Fl(a){var b=a.csn;a=a.root_ve_type;if(b&&a&&(fj(b,a),a=aj()))for(var c in Cl){var d=Cl[c];d&&xj(b,a,[d])}}
function Gl(a){Cl[a.id]=new Xi({trackingParams:a.tracking_params})}
function Hl(a){var b=cj(),c=Cl[a.id];if(b&&c){a=R("use_default_events_client")?void 0:Ci;var d="INTERACTION_LOGGING_GESTURE_TYPE_GENERIC_CLICK";c={csn:b,ve:c.getAsJson(),gestureType:d};d={ia:ej(b),V:b};"UNDEFINED_CSN"==b?yj("visualElementGestured",c,d):a?Wg("visualElementGestured",c,a,d):Di("visualElementGestured",c,d)}}
function Il(a){a=a.ids;var b=cj();if(b)for(var c=0;c<a.length;c++){var d=Cl[a[c]];if(d){var e=b,f=R("use_default_events_client")?void 0:Ci;d={csn:e,ve:d.getAsJson(),eventType:1};var g={ia:ej(e),V:e};"UNDEFINED_CSN"==e?yj("visualElementShown",d,g):f?Wg("visualElementShown",d,f,g):Di("visualElementShown",d,g)}}}
;x("yt.setConfig",P,void 0);x("yt.config.set",P,void 0);x("yt.setMsg",Ef,void 0);x("yt.msgs.set",Ef,void 0);x("yt.logging.errors.log",Mi,void 0);
x("writeEmbed",function(){var a=Q("PLAYER_CONFIG",void 0);if(!a){var b=Q("PLAYER_VARS",void 0);b&&(a={args:b})}Fk(!0);"gvn"==a.args.ps&&(document.body.style.backgroundColor="transparent");a.args.forced_experiments||(b=window.location.href,-1!=b.indexOf("?")?(b=(b||"").split("#")[0],b=b.split("?",2),b=bh(1<b.length?b[1]:b[0])):b={},b.forced_experiments&&(a.args.forced_experiments=b.forced_experiments));a.attrs||(a.attrs={width:"100%",height:"100%",id:"video-player"});var c=document.referrer;b=Q("POST_MESSAGE_ORIGIN");
window!=window.top&&c&&c!=document.URL&&(a.args.loaderUrl=c);zl=a=(c=Q("WEB_PLAYER_CONTEXT_CONFIGS",void 0))&&"WEB_PLAYER_CONTEXT_CONFIG_ID_EMBEDDED_PLAYER"in c?Yj(a,c.WEB_PLAYER_CONTEXT_CONFIG_ID_EMBEDDED_PLAYER,!1):Yj(a);a.addEventListener("onScreenChanged",Fl);a.addEventListener("onLogClientVeCreated",Dl);a.addEventListener("onLogServerVeCreated",Gl);a.addEventListener("onLogVeClicked",Hl);a.addEventListener("onLogVesShown",Il);a.addEventListener("onVideoDataChange",El);c=Q("POST_MESSAGE_ID","player");
Q("ENABLE_JS_API")?Bl=new qk(a):Q("ENABLE_POST_API")&&"string"===typeof c&&"string"===typeof b&&(Al=new rk(window.parent,c,b),Bl=new lk(a,Al.h));kg()},void 0);
var Jl=Af(function(){vl("ol");var a=Gk.getInstance(),b=!!((Jk("f"+(Math.floor(119/31)+1))||0)&67108864),c=1<window.devicePixelRatio;if(document.body&&Ld(document.body,"exp-invert-logo"))if(c&&!Ld(document.body,"inverted-hdpi")){var d=document.body;if(d.classList)d.classList.add("inverted-hdpi");else if(!Ld(d,"inverted-hdpi")){var e=Jd(d);Kd(d,e+(0<e.length?" inverted-hdpi":"inverted-hdpi"))}}else!c&&Ld(document.body,"inverted-hdpi")&&Md();b!=c&&(b="f"+(Math.floor(119/31)+1),d=Jk(b)||0,d=c?d|67108864:
d&-67108865,0==d?delete Y[b]:(c=d.toString(16),Y[b]=c.toString()),a.save())}),Kl=Af(function(){var a=zl;
a&&a.sendAbandonmentPing&&a.sendAbandonmentPing();Q("PL_ATT")&&Qi.dispose();a=0;for(var b=ig.length;a<b;a++){var c=ig[a];if(!isNaN(c)){var d=y("yt.scheduler.instance.cancelJob");d?d(c):U(c)}}ig.length=0;cg("//static.doubleclick.net/instream/ad_status.js");jg=!1;P("DCLKSTAT",0);Uc(Bl,Al);if(a=zl)a.removeEventListener("onScreenChanged",Fl),a.removeEventListener("onLogClientVeCreated",Dl),a.removeEventListener("onLogServerVeCreated",Gl),a.removeEventListener("onLogVeClicked",Hl),a.removeEventListener("onLogVesShown",
Il),a.removeEventListener("onVideoDataChange",El),a.destroy();Cl={}});
window.addEventListener?(window.addEventListener("load",Jl),window.addEventListener("unload",Kl)):window.attachEvent&&(window.attachEvent("onload",Jl),window.attachEvent("onunload",Kl));Ua("yt.abuse.player.botguardInitialized",y("yt.abuse.player.botguardInitialized")||Ri);Ua("yt.abuse.player.invokeBotguard",y("yt.abuse.player.invokeBotguard")||Si);Ua("yt.abuse.dclkstatus.checkDclkStatus",y("yt.abuse.dclkstatus.checkDclkStatus")||lg);
Ua("yt.player.exports.navigate",y("yt.player.exports.navigate")||gj);Ua("yt.util.activity.init",y("yt.util.activity.init")||xg);Ua("yt.util.activity.getTimeSinceActive",y("yt.util.activity.getTimeSinceActive")||Ag);Ua("yt.util.activity.setTimestamp",y("yt.util.activity.setTimestamp")||yg);}).call(this);
