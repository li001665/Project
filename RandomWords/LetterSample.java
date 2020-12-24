import java.lang.Character;
import java.lang.String;
//Jianhua Li
//5598966
public class LetterSample {
    /*
      represent the letter sample as an array
     */
    private String str;
    private char letter;
    public static final char STOP = '.'; // a static property
    // a constructor which takes the segment string and the nextLetter char 
    public LetterSample(String str, char letter){
        this.str = str;
        this.letter = letter;
    }
    //get the segment string
    public String getSegment(){
        return this.str;
    }
    //get the nextLetter char
    public char getNextLetter(){
        return this.letter;
    }
    //output
    public String toString(){
        return "\"" + getSegment() + "\"" + " -> " + getNextLetter();  
    }
    //take a string and generate letter samples from it
    public static LetterSample[] toSamples(String input, int segmentSize){
        String newstr = "";
        for(int i=0;i<input.length();i++){
            if(Character.isAlphabetic(input.charAt(i))){
                if(Character.isUpperCase(input.charAt(i)))
                    newstr += Character.toLowerCase(input.charAt(i));
                else
                    newstr += input.charAt(i); 
            }
        }
        newstr += STOP;
        LetterSample[] array = new LetterSample[newstr.length()];
        for(int i=0;i<array.length;i++){
            if(i == 0)
                array[i] = new LetterSample("", newstr.charAt(i));
            else{
                if(segmentSize == 0)
                    array[i] = new LetterSample("", newstr.charAt(i));
                else if(array[i-1].getSegment().length()< segmentSize)
                    array[i] = new LetterSample(array[i-1].getSegment() + array[i-1].getNextLetter(), newstr.charAt(i));
                else{
                    array[i] = new LetterSample(array[i-1].getSegment().substring(1) + array[i-1].getNextLetter(), newstr.charAt(i));
                    if(i == array.length - 1)
                        return array;
                } 
            }
        }
        return array;  

    }


}