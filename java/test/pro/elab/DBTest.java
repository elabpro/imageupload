/*
 * Тест для обертки БД
 */
package pro.elab;

import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author eugine
 */
public class DBTest {

    public DBTest() {
    }

    /**
     * Test for class DB
     */
    @Test
    public void testCounter() {
        System.out.println("setCounter");
        int v = 0;
        DB db = new DB();
        int expResult = 0;
        int result = db.setCounter(v);
        assertEquals(expResult, result);
        result = db.getCounter();
        assertEquals(v + 1, result);
        result = db.getCounter();
        assertEquals(v + 2, result);
        v = 10;
        result = db.setCounter(v);
        assertEquals(v, result);
        result = db.getCounter();
        assertEquals(v + 1, result);
        result = db.getCurrentCounter();
        assertEquals(v + 1, result);
    }

}
