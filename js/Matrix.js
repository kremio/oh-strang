let OhStrang = require('./oh-strang').OhStrang


let NumberMatrix = OhStrang.DoubleMatrix.prototype;

NumberMatrix.setValues = function(values){
	let i = 0
	for(let r = 1; r <= this.getRowsCount(); r++){
		for(let c = 1; c <= this.getColumnsCount(); c++){
			this.setValue( r, c, values[i++] )
		}
	}
}


NumberMatrix.getValues = function(){
	let i = 0
	let values = []
	for(let r = 1; r <= this.getRowsCount(); r++){
		for(let c = 1; c <= this.getColumnsCount(); c++){
			values[i++] = this.getValue( r, c )
		}
	}
	return values
}



NumberMatrix.toString = function(){
	return this.asString()
}


NumberMatrix.inspect = function(){
	return this.toString()
}


module.exports = OhStrang.DoubleMatrix
