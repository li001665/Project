//Jianhua Li
//5598966
public class Gibberisher {
    /*
      implements the primary random word generation algorithm 
     */
    private Trie<CharBag> trie;
    private int segmentSize;
    private int sampleCount;
    // constructor initialize these three variables
    public Gibberisher(int segmentSize){
        this.segmentSize = segmentSize;
        trie = new Trie<>();
        sampleCount = 0;
    }
    //add one sample into the model
    public void train(LetterSample sample){
        if(trie.get(sample.getSegment()) == null){
            CharBag charBag = new CharBag();
            charBag.add(sample.getNextLetter());
            trie.put(sample.getSegment(), charBag);
        }
        else
            trie.get(sample.getSegment()).add(sample.getNextLetter());
    }
    //generate LetterSamples, then train the model for each Letter Sample
    public void train(String input){
        LetterSample[] array = LetterSample.toSamples(input, segmentSize);
        for(int i=0;i<array.length;i++){
            train(array[i]);
        }
    }
    //call the preceding train(Strring) method
    public void train(String[] input){
        for(int i=0;i<input.length;i++){
            this.train(input[i]);
        }
    } 
    //get the number of the samples we used
    public int getSampleCount(){
        return sampleCount;
    }
    //generate a string
    public String generate(){
        String word = "";
        char nextLetter = 'a';
        String sample = "";
        while (nextLetter != '.'){
            if(word.length() <= segmentSize)
                sample = word;
            else
                sample = word.substring(word.length() - segmentSize);
                nextLetter = trie.get(sample).getRandomChar(); 
            word += nextLetter;
        }
        return word;  
    }


}