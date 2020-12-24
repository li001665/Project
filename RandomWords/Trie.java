//Jianhua Li
//5598966
public class Trie<T>{
    /*
     provide an easy-to-use interface on the data storage to keep TrieNodes organized
     */
    private TrieNode<T> trieNode;
    //constructor
    public Trie(){
        trieNode = new TrieNode<T>();
    }
    //take a string and return the appropriate trieNode
    private TrieNode<T> getNode(String input){
        TrieNode<T> newNode = new TrieNode<>();
        TrieNode<T> middle = trieNode;
        //if(input == "")
        //    newNode = middle.getChild('A');  //set empty char to a Uppercase Character
        for(int i=0;i<input.length();i++){
            newNode = middle.getChild(input.charAt(i));
            middle = newNode;
        }
        return middle;
    }
    //get the data currently stored on the TrieNode associated with a input string
    public T get(String input){
        TrieNode<T> newNode = this.getNode(input);
        return newNode.getData();
    }
    //set the data currently stored on the TrieNode associated with the input string to the T value provided
    public void put(String input, T data){
        TrieNode<T> newNode = this.getNode(input);
        newNode.setData(data);
    }
    //return the root node
    public TrieNode<T> getRoot(){
        return trieNode;
    }


}