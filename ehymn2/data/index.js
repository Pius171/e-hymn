var options = {
    valueNames: [ 'name', 'number' ],
    item: '<li><h3 class="name"></h3><p class="number"></p></li>'
  };
  
  var values = [];
  
  var userList = new List('users', options, values);
  


  function hello(paran) {
    let hymnLength = Object.keys(paran).length;
    for(const key in paran) {
        console.log(key + "" + paran[key])
        userList.add({
            name: key,
            number: paran[key]
        });
    }
  }
  let hymns, stat;
  
  fetch("./hymns.json")
  .then(response => {
    stat = response.ok;
     return response.json();
  })
  .then(data =>{
    // call a function
    if(stat) {
        hymns = data;
        //call function
        hello(hymns);
    }
  })

  // let search_input = document.querySelector(".search");
 
  // search_input.addEventListener("keydown", function(){
  //   document.querySelector(".list").classList.remove("inactive")
  // })
 
    
