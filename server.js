const app = require('./app')
const PORT=process.env.PORT || 3000;
const socket = require('socket.io');


const server = app.listen(PORT, ()=>{
  console.log('hello');
});

const io=socket(server);

io.on('connect', function(socket){

  console.log('un cliente se conecto'+ ' ' +socket.id);

  socket.on('message', function(message){
    console.log(message)
  })



});