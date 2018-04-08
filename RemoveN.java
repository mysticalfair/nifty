public class RemoveN {
    public ListNode move(ListNode list, int remove, int n) {
        // replace statement below with code you write
        if(list==null)
        return null;
        ListNode target=list;
      ListNode append=list;
      ListNode splice=list;
      ListNode fin=append;
        while(target.next!=null)
        {
          if(target.info==remove)
          {target=target.next;
          break;}
          target=target.next;
          append=append.next;
          splice=splice.next;

        }

        if(target.next==null)
          return list;//returns original if the target was not found
        //now target should be the one to remove the nodes after


        for(int k=0; k<n; k++)
        {
          splice=splice.next;
        }
        append.next=splice.next;
        while(append.next!=null)
        {
          append=append.next;
        }
        append.next=target;
        for(int i=0; i<n; i++)
            append=append.next;
        append.next=null;


        return fin;

    }
}
