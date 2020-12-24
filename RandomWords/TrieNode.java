//Jianhua Li
//5598966
public class TrieNode<T> {
    /*
       build a Tree 
     */
    private T currentVal;
    private TrieNode<T>[] nextNode =(TrieNode<T>[]) new TrieNode[26];
    //constructor initialize to null
    public TrieNode(){
        currentVal = null;
        for(int i=0;i<26;i++)
            nextNode[i] = null;
    }
    // get the data
    public T getData(){
        return currentVal;
    }
    //set the data
    public void setData(T data){
        currentVal = data;
    }
    //return TrieNode<T> with the given letter
    public TrieNode<T> getChild(char letter){
        if(!Character.isLowerCase(letter))
            return null;
        else{
            int index = (int)(letter - 'a');
            if(nextNode[index] == null){
                nextNode[index] = new TrieNode<T>();
                return nextNode[index];
            }
            else
                return nextNode[index];
        }
    }
    // get the size of the tree
    public int getTreeSize(){
        int count = 1;
        for(int i=0;i<26;i++){
            if(nextNode[i] == null)
                count += 0;    
            else if(nextNode[i] != null)
                count += nextNode[i].getTreeSize();           
        }
        return count;
    }
        

}