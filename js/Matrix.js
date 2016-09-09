let Module = require('./oh-strang')


let NumberMatrix = Module.DoubleMatrix.prototype;
	
NumberMatrix.setValues = function(values){
	let i = 0;
	for(let r = 1; r <= this.getRowsCount(); r++){
		for(let c = 1; c <= this.getColumnsCount(); c++){
			this.setValue( r, c, values[i++] )
		}
	}
}

NumberMatrix.toString = function(){
	return this.asString()
}


NumberMatrix.inspect = function(){
	return this.toString()
}


module.exports = Module.DoubleMatrix
