var options = {
    valueNames: [ 'name', 'number' ],
    item: '<li><span class="number"></span><span class="name"></span></li>'
  };
  
  var values = [];
  
  var userList = new List('users', options, values);
  


  function hello(paran) {
    let hymnLength = Object.keys(paran).length;
    for(const key in paran) {
        
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
 
async function redirect(e){
  console.log(e.target);
  e.preventDefault();

  let allData = new FormData(e.target)
  // const stat = await fetch("")

  console.log(allData)
}
 
const hymnSelect = document.querySelector("#hymnlist");
hymnSelect.addEventListener("submit", redirect)

    
