//Jianhua Li
//5598966
import java.util.Random;
public class CharBag {
    /*
       Bag Abstract Data Type, storing elemnets with no order, and a letter can appear many times.
     */

    //an array to store counts
    private int[] array = new int[27];

    public static final char STOP = '.';
    
    //default constructor
    public CharBag(){}

    //add a char to the charBag
    public void add(char input){
        char letter;
        int index = -1;
        if(Character.isAlphabetic(input)){
            if(Character.isUpperCase(input))
                letter = Character.toLowerCase(input);
            else
                letter = input;
            index = (int)letter - (int)('a');
            array[index]++;
        }
        else
            array[26]++;

    }

    //remove a char from the CharBag
    public void remove(char c){
        int index = -1;
        if(Character.isAlphabetic(c)){
            if(Character.isUpperCase(c))
                c = Character.toLowerCase(c);
            index = (int)c - (int)('a');
            if(array[index] != 0)
                array[index]--;
        }
        else{
            if(array[26] != 0)
                array[26]--;
        }
    }

    //get how many tomes a given char is in the CharBag
    public int getCount(char c){
        int index = -1;
        if(Character.isAlphabetic(c)){
            if(Character.isUpperCase(c))
                c = Character.toLowerCase(c);
            index = (int)c -(int)('a');
            return array[index];   
        }
        else
            return array[26];
    }

    // return the total size of the CharBag
    public int getSize(){
        int size = 0;
        for(int i=0;i<27;i++){
            size += array[i];
        }
        return size;
    }

    //toString method
    public String toString(){
        String output = "CharBag{";
        char letter;
        for(int i=0;i<27;i++){
            if(i == 26){
                output += STOP + ":" + array[i] + "}";
                return output;
            }
            letter = (char)((int)('a') + i);
            output += letter + ":" + array[i] + ", ";
        }
        return output;
    }

    //return a randomly chosen char from the chars in the char bag
    public char getRandomChar(){
        Random r = new Random();
        int count = 0;
        if(getSize() > 0)
            count = r.nextInt(getSize());
        for(int i=0;i<26;i++){
            count -= array[i];
            if(count < 0)
                return (char)((int)('a') + i);
        }
        return STOP;
    }
    
    
}